#pragma once
#include "errcodes.hpp"

#include "../Source/tree.cpp"
#include "settings.hpp"
#include "token.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "debug.hpp"
#include <cmath>
#include <fstream>



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
  public:
	char* text;
	unsigned long long int text_length;
	//Token* source_tokens;
	//unsigned long long int tokens_length;
	//PTree programm_tree;
	
	int importLibraries();
	int setDefinitions();
	int deleteComments();
	
	
  public:
	Programm();
	~Programm();
	
	int readSource(const Settings* settings);
	int preprocessor(const Settings* settings);
	int makeTokens(const Settings* settings);
	int makeTree(const Settings* settings);
	int optimize(const Settings* settings);
	int compile(const Settings* settings);
	int write(const Settings* settings);
};


int strcount(char* str, const char* expression);
bool isSpace(char symbol);
int getHash(char*);

