#include <iostream>
using namespace std;
#include "Tree.h"
#include <string>
#include <fstream>
#include <time.h>
int main()
{
	remove ("Result.txt");
	double start, end;
	Tree A;
	ifstream infile;
	string data = "datas.txt";
	int n = 0;
	infile.open(data);
	cout << " Loading .... " << endl;
	start = clock();
	if ( infile.is_open())
	{
	string file_name;
	while ( infile >> file_name )
		A.Load_List_From_File( file_name + ".txt", n);
	}
	else cout << " Cant open file " << endl;
	infile.close();
	end = clock();
	double insertion = end - start;

	ofstream Infile;
	Infile.open("Result.txt", std::ios::app);
	++n;
	if ( Infile)
	{
		start = clock();
		A.Read_Query();
		end = clock();
		Infile << "--------------------------------------------------" << endl;
		Infile << "Insertion time : " << insertion << "ms" << endl;
		Infile << "Total Search time : " << end - start << "ms" << endl;
	}
	else cout << " Can't create new file " << endl;
	Infile.close();
	cout << " DONE!!! " << endl;
	system("pause");
}
