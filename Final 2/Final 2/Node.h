#include<iostream>
using namespace std;
 struct Node
{
	bool is_word[224];
	Node * Link[36];

	Node ();
	~Node();
};