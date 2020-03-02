#pragma once
#include "token.hpp"

typedef Tree<Token*> PTree
typedef Ptree::Node PNode

class Programm
{
  private:
	char* source_path;
	char* output_path;
	char* source_text;
	unsigned long long int text_length;
	Token* source_tokens;
	unsigned long long int tokens_length;
	PTree programm_tree;
	char* compiled_text;	
	
	
  public:
  	Programm();
	Programm(int argc, char* argv[]);
	~Programm();
	
	int readSource();
	int makeTokens();
	int makeTree();
	int optimize();
	int compile();
	int write()
};
