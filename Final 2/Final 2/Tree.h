#include <iostream>
#include <fstream>
#include <string>
#include "Node.h"
#include <stack>
#include <queue>
 class Tree
{
	Node* _head;
public:
	Tree( Node* head = NULL );
	~Tree();
	void Add_Node ( char * word, int length, int count, int n);
	bool * Search_Node(std::string target, int length, int count);
	void Load_List_From_File(std::string file_name, int &n);

	void Read_Query();
	void Execute_order ( std::stack<int> &order, std::stack<bool*> &myResult);
	void Print_Result( string File_Name, bool * final_result, int &end, int &start, int &n , std::queue<string> &myword);
	bool * AND ( bool * array1, bool * array2 );
	bool * OR ( bool * array1, bool * array2);
	bool * NOT( bool * array1, bool *array2);

 };
