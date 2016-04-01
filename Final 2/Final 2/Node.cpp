#include "Node.h"
Node::Node()
{
	memset (Link, NULL, 36*4);
	memset(is_word,false,224);
}
Node::~Node()
{
	for ( int i = 0; i < 36; ++i)
	{
		if ( Link[i] )
			delete Link[i];
	}
}