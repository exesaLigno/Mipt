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
	int status = OK;
	
  private:
	char* text_pointer = nullptr;
	
  public:
	short int source_type = 0;
	AST* ast = nullptr;
	
	Source();
	Source(const char* name);
	~Source();
	
	bool open();
	short int getType();
	void print();
	
	void makeAST();
	void optimizeAST();
	void prepareAST();
	
	void dumpAST();
	
  private:
	int calculateIndent(char* text);
	bool nextLine(char** _text);
	
	ASN* parseBlock(int indent, char** _text);
	ASN* parseLine(int indent, char** _text);
	ASN* getInclude(int indent, char** _line);
	ASN* getDef(int indent, char** _line);
	ASN* getAssignment(int indent, char** _line);
	ASN* getOperators(int indent, char** _line);
	ASN* getLogic(int indent, char** _line);
	ASN* getCmp(int indent, char** _line);
	ASN* getAddSub(int indent, char** _line);
	ASN* getMulDiv(int indent, char** _line);
	ASN* getPow(int indent, char** _line);
	ASN* getNumVarFunc(int indent, char** _line);
	ASN* getItemize(int indent, char** _line);
	
	void splitFunctions();
	void enumerateMembers();
	
	void enumerateBranching(ASN* node, int* number);
	char* getUnnumeratedVariable(ASN* node);
	void setVariables(ASN* node, const char* varname, int vartype, int varnumber);
	
	void substituteStatic(ASN* node);
	
	void foldConstants(ASN* node);
	void foldArithmeticConstants(ASN* node);
	void foldCmpConstants(ASN* node);
	void foldCtrlConstants(ASN* node);
	
	void inlineFunctions(ASN* node);
	
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
		OK,
		OPENNING_PROHIBITED,
		FILE_NOT_EXIST,
		UNSUPPORTED_FILE_EXTENSION,
		NOT_SUBSTITUTABLE_TYPE,
		SYNTAX_ERROR
	};
};

