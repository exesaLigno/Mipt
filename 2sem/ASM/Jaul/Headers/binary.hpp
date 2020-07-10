#pragma once

#include <cstring>
#include "ast.hpp"
#include "source.hpp"

class Binary
{
  public:
  	class Token;
	class Header;
	
	Source* source = nullptr;
	
	Token* start = nullptr;
	Token* end = nullptr;
	
	Binary();
	Binary(Source* source);
	~Binary();
	
	int compileAst();		// push back and generate byte code
	int compileNasm();		// push back and generate byte code
	int importObj();		// manual - taking name of func, byte code of func and creating new token
	
	int pushBack(const char* nasm_code);
	
	int exportNasm();
	int exportObj();
	int exportExecutable();
	int exportVirtualExecutable();
};


class Binary::Token
{
	friend Binary;
	
  protected:
	char* label = nullptr;
	short int label_type = 0;
	
	char* nasm_code = nullptr;
	char* byte_code = nullptr;
	int byte_code_length = 0;
	long long int first_byte_position = 0;
	
	Token* prev = nullptr;
	Token* next = nullptr;
	
	enum LabelTypes{EMPTY, LOCAL, GLOBAL, FUNCTION};
	
  public:
	Token();
	Token(const char* name, const char* nasm_text);
	int setLabel(const char* name, int name_length);
	int setBytes(const char* bytes, long int bytes_count);
	~Token();
	
	int toNasm();	// if byte code given, generating NASM code
	int toByte();	// the same, but from NASM to byte code
};


class Binary::Header
{
	friend Binary;
  
  protected:
	Header();
	~Header();
};
