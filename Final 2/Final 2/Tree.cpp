#include "Tree.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <time.h>
#include <stack>
#include <queue>
#pragma once
using namespace std;
Tree::Tree( Node* head )
	:_head(head) {};

Tree::~Tree()
{
	if (_head)
	delete _head;
}

void Tree::Add_Node( char * word, int length, int  count, int n )
{
	if ( !_head)
		_head = new Node;
	Node* temp = _head;
	while ( count < length)
	{
		if ( word[count] >= 'a'  && word[count] <= 'z'  )
		{
			if ( !temp->Link[int(word[count]) - 'a'] )
				temp->Link[int(word[count]) - 'a' ] = new Node;
			temp = temp->Link[int(word[count]) - 'a'];
		}
		else if ( word[count] >= 'A' && word[count] <= 'Z'  )
		{
			if ( !temp->Link[int(word[count]) - 'A'] )
				temp->Link[int(word[count]) - 'A'] = new Node;
			temp = temp->Link[int(word[count]) - 'A'];
		}
		else if ( word[count] >= '0' && word[count] <= '9' )
		{
			if ( !temp->Link[int(word[count]) - 22] )
				temp->Link[int(word[count]) - 22] = new Node;
			temp = temp->Link[int(word[count]) - 22];
		}
		++count;
	}
	temp->is_word[n] = true;
}

bool * Tree::Search_Node(string target,int length, int count)
{
		Node* temp = _head;
	while ( count < length)
	{
		if ( target[count] >= 'a'  && target[count] <= 'z'  )
			temp = temp->Link[int(target[count]) - 'a'];
		else if ( target[count] >= 'A' && target[count] <= 'Z'  )
			temp = temp->Link[int(target[count]) - 'A'];
		else if ( target[count] >= '0' && target[count] <= '9' )
			temp = temp->Link[int(target[count]) - 22];
		if ( temp == NULL ) return NULL;

		++count;
	}

	for ( int i = 0; i < 224; ++i )
	{
		if ( temp->is_word[i] == true) 
			return temp->is_word;
	}
	return NULL;
}

void Tree::Load_List_From_File( std::string file_name, int &n)
{
	FILE * in;
	in = fopen(file_name.c_str(), "rb");
	unsigned long long inputSize;
	char * temp;

	if (in) {
		fseek(in, 0, SEEK_END);
		inputSize = ftell(in);
		rewind(in);
		temp = new char[inputSize+1];
		fread(temp, 1, inputSize, in);
		fclose(in);
		
	}
	temp[inputSize] = '\0';
	char * todelete = temp;
	int c = 0;
	for ( int i = 0; i < inputSize; ++i)
	{
		if (!( 64 < temp[c] && temp[c] < 123 || 47 < temp[c] && temp[c] < 58 ) && i < inputSize)
		{
			while (!( 64 < temp[c] && temp[c] < 123 || 47 < temp[c] && temp[c] < 58 ) && i < inputSize)
			{
				++c;
				++i;
			}
			temp[c-1] = '\0';
			temp = &temp[c];
		}
		else
		{
			while ( 64 < temp[c] && temp[c] < 123 || 47 < temp[c] && temp[c] < 58 )
			{
				++c;
				++i;
			}
			temp[c] = '\0';
			if ( temp != "" )
				Add_Node( temp, c, 0, n);
			temp = &temp[c+1];
			++i;
		}
		c = 0;
	}
	delete[] todelete;
	++n;
}

void Tree::Read_Query()
{
	std::queue<string> myword;
	std::stack<int> trash;
	std::stack<int> order;
	std::stack<bool*> myResult;
	string file_name = "query.txt";
	ifstream infile;
	infile.open("query.txt");
	string word;
	int n = 0;
	int start,end;
	int flag = 0;
	bool * final_result = NULL;
	if ( infile.good() )
	{
		start = clock();
		while ( infile >> word)
		{
			if ( word == "AND")
			{
				--flag;
				order.push(1);
				myword.push("AND");
			}
			else if ( word == "OR" )
			{
				--flag;
				int i = 0;
				while ( order.size() > 0 )
				{
					if ( order.top() < 2 ) 
					{
						order.pop();
						++i;
					}
					else break;
				}
				order.push(2);
				myword.push("OR");
				while ( i > 0) 
				{
					order.push(1);
					--i;
				}
			}
			else if (word == "NOT" )
			{
				--flag;
				while ( order.size() > 0 )
				{
					if ( order.top() < 3 )
					{
						trash.push(order.top());
						order.pop();
					}
					else break;

				}
				order.push(3);
				myword.push("NOT");
				while ( trash.size() > 0 )
				{
						order.push(trash.top() );
						trash.pop();
				}
			}
			else
			{
					++flag;
					if (flag > 1 ) 
					{
						if (order.size() > 0 )
							Execute_order(order, myResult);
						Print_Result("Result.txt", myResult.top() , end, start, n, myword);
						myResult.pop();
						start = clock();
						flag = 1;
					}
					myword.push(word);
					myResult.push(Search_Node(word,word.length(), 0));
					if ( myResult.size() >= 2 )
					{
						if ( order.size() >= 1 )
						{
							if ( myResult.size() - order.size() == 1 )
							{
								if ( order.top() == 1 )
								{
									bool* Result1 = myResult.top();
									myResult.pop();
									bool* Result2 = myResult.top();
									myResult.pop();
									myResult.push(AND(Result1, Result2));
									order.pop();
								}
							}
						}
					}
			}
		}

	}
		if (order.size() > 0 )
			Execute_order(order, myResult);
		Print_Result("Result.txt", myResult.top() , end, start, n, myword);
		myResult.pop();
	infile.close();
}
void Tree::Execute_order( stack<int> &order, stack<bool*> &myResult)
{
		while ( myResult.size() != 1 )
		{
			bool* Result1 = myResult.top();
			myResult.pop();
			bool* Result2 = myResult.top();
			myResult.pop();
			if ( order.top() == 2)
			{
					myResult.push(OR(Result1, Result2));
					order.pop();
			}
			else if ( order.top() == 3 )
			{
					myResult.push(NOT(Result2,Result1) );
					order.pop();
			}
			else
			{
					myResult.push(AND(Result1,Result2) );
					order.pop();
			}
		}
}
void Tree::Print_Result( string File_name, bool * final_result, int &end, int &start, int &n, std::queue<string> &myword) 
{
	ofstream Infile;
	Infile.open(File_name, std::ios::app);
	end = clock();
	++n;
	if ( Infile)
	{
		Infile << "--------------------------------------------------" << endl;
		Infile << "Query " << n << " : ";
		int size = myword.size();
		for ( int i = 0; i < size ; ++i )
		{
			Infile << myword.front() << " ";
			myword.pop();
		}
		Infile << endl;
		Infile << "Time :" << end - start << "ms"<< endl;
		Infile << "Files contain : " << endl;
		int t = 0;
		for ( int i =0; i < 224; ++i)
		{
			if ( final_result[i] == true )
				Infile << "d00" << i + 1 << ".txt"<< endl;
		}
		Infile << endl;
	}
	else cout << " Can't create new file " << endl;
	Infile.close();
}
bool *Tree::AND(bool * array1, bool * array2)
{
	bool * result = new bool[224];
	memset(result, false, 224);
	if ( array1 && array2 )
	{
		for ( int i = 0; i < 224; ++i )
		{
			if ( array1[i] && array2[i] )
				result[i] = true;
		}
	}
	return result;
}
bool *Tree::OR (bool * array1, bool * array2)
{
	bool * result = new bool[224];
	memset(result, false, 224);
	if ( !array1 && array2 )
	{
		for ( int i = 0; i < 224; ++i )
		{
		if (  array2[i] )
			result[i] = true;
		}
	}
	else if ( !array2 && array1 )
	{
		for ( int i = 0; i < 224; ++i )
		{
			if ( array1[i]  )
				result[i] = true;
		}
	}
	else
	{
		for ( int i = 0; i < 224; ++i )
		{
			if ( array1[i] || array2[i] )
				result[i] = true;
		}
	}
	return result;
}
bool *Tree::NOT (bool * array1, bool * array2)
{
	bool * result = new bool[224];
	memset(result, false, 224);
	if ( !array1 ) return result;
	else if ( !array2 )
	{
		for ( int i = 0; i < 224; ++i )
		{
			if ( array1[i])
			result[i] = true;
		}
	}
	else
	{
		for ( int i = 0; i < 224; ++i )
		{
			if ( array1[i] && !array2[i] )
				result[i] = true;
		}
	}
	return result;
}