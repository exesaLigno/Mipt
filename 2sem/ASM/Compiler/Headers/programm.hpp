#pragma once
#include "errcodes.hpp"

#include "../Source/tree.cpp"
#include "token.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>

typedef Tree<Token*> PTree;
typedef PTree::Node PNode;



class Programm
{
  private:
	char* source_path;
	char* output_path;
	char* source_text;
	unsigned long long int source_length;
	Token* source_tokens;
	unsigned long long int tokens_length;
	PTree programm_tree;
	char* compiled_text;
	bool asm_listing;
	
	
  public:
	Programm();
	Programm(int argc, char* argv[]);
	~Programm();
	
	int readSource();
	int makeTokens();
	int makeTree();
	int optimize();
	int compile();
	int write();
};
