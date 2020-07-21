/*!
 *	@file source.hpp
 *	@brief Описание класса исходного кода
 */


#pragma once

#include <cstring>
#include <fstream>
#include "ast.hpp"
#include "debug.hpp"

class Source
{
  public:
	char* name = nullptr;
	char* text = nullptr;
	long long int text_length = 0;
	int current_line = 1;
	
  private:
	char* text_pointer = nullptr;
	
  public:
	short int source_type = 0;
	AST* ast = nullptr;
	
	Source();
	Source(const char* name);
	~Source();
	
	short int open();
	short int getType();
	void print();
	
	void makeAST();
	
	void rebuildTree();
	void prepareAST();
	void dumpAST();
	
  private:
	int calculateIndent(char* text);
	bool nextLine(char** _text);
	
	ASN* parseBlock(int indent, char** _text);
	ASN* parseLine(int indent, char** _text);
	ASN* getInclude(char** _line);
	ASN* getDef(char** _line);
	ASN* getAssignment(char** _line);
	ASN* getOperators(char** _line);
	ASN* getLogic(char** _line);
	ASN* getCmp(char** _line);
	ASN* getAddSub(char** _line);
	ASN* getMulDiv(char** _line);
	ASN* getPow(char** _line);
	ASN* getNumVarFunc(char** _line);
	ASN* getItemize(char** _line);
	
	void enumerateBranching(ASN* node, int* number);
	char* getUnnumeratedVariable(ASN* node);
	void setVariables(ASN* node, const char* varname, int vartype, int varnumber);
	
  public:
	
	enum SourceTypes
	{
		JAUL_SOURCE = 1,		// .j		| Simple hi-level source
		JASM_SOURCE,			// .jasm	| Special assembler (like nasm, but without macroses etc)
		JAUL_OBJ,				// .jo		| Object format
		VIRTUAL_EXECUTABLE,		// .jv		| Virtual executable format
		DEFINITION,				// fileless	| Not implemented fuck
		ERRTYPE
	};
	
	enum Errors
	{
		OK = 0,
		OPENNING_PROHIBITED = -1,
		FILE_NOT_EXIST = -2,
		UNSUPPORTED_FILE_EXTENSION = -3,
		NOT_SUBSTITUTABLE_TYPE = -4
	};
};

