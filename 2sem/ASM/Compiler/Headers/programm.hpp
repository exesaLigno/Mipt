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
#include <cctype>


#define skip_spaces(where)	while (*where == ' ')	\
								(where)++;			\


typedef Tree<char*> PTree;
//typedef Tree<Token*> PTree;
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

int calculateIndent(char* text);
bool nextLine(char** _text);
PNode* parseBlock(int indent, char** _text);
PNode* parseLine(int indent, char** _text);

PNode* getDef(char** _line);

PNode* getAssignment(char** _line);
PNode* getBranching(char** _line);
PNode* getLogic(char** _line);
PNode* getCmp(char** _line);
PNode* getAddSub(char** _line);
PNode* getMulDiv(char** _line);
PNode* getPow(char** _line);
PNode* getNumVarFunc(char** _line);

PNode* getItemize(char** _line);

