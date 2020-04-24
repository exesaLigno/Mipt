#pragma once

#include "../Source/tree.cpp"
#include "settings.hpp"
#include "token.hpp"
#include "library.hpp"
#include "definition.hpp"
#include "debug.hpp"
#include "errcodes.hpp"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>



typedef Tree<Token*> PTree;
typedef PTree::Node PNode;


#define WRONGINDENT -2
#define EMPTYLINE -1



class Programm
{
  public:
	char* text;
	unsigned long long int text_length;
	//Token* source_tokens;
	//unsigned long long int tokens_length;
	PTree programm_tree;
	
	int importLibraries();
	int setDefinitions();
	int deleteComments();
	
	PNode* parseBlock(int indent, char** _text);
	PNode* parseLine(char** _text);
	
	
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

