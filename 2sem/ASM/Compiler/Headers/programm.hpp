#pragma once
#include "errcodes.hpp"

#include "../Source/tree.cpp"
#include "token.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "debug.hpp"
#include <cmath>



typedef Tree<Token*> PTree;
typedef PTree::Node PNode;


class Definition
{
  public:
	char* defname;
	char* defstatement;
	int defname_length;
	int defstatement_length;
	
	Definition();
	~Definition();
};

class Library
{
  public:
	char* libname;
	int libname_length;
	char* libtext;
	unsigned long long int libtext_length;
	
	Library();
	~Library();
	int readLibrary();
};



class Programm
{
  private:
	char* source_path;
	char* output_path;
	char* source_text;
	unsigned long long int source_length;
	Library* libraries;
	int libraries_count;
	Definition* definitions;
	int definitions_count;
	Token* source_tokens;
	unsigned long long int tokens_length;
	PTree programm_tree;
	char* compiled_text;
	bool asm_listing;
	bool silent;
	
	int importLibraries();
	int setDefinitions();
	
	
  public:
	Programm();
	Programm(int argc, char* argv[]);
	~Programm();
	
	int readSource();
	int preprocessor();
	int makeTokens();
	int makeTree();
	int optimize();
	int compile();
	int write();
};


int strcount(char* str, const char* expression);
bool isSpace(char symbol);
int getHash(char*);

