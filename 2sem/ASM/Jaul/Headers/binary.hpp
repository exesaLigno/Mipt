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
	
	Binary(Source* source);
	~Binary();
	
	void compileAst();		// push back and generate byte code
	int compileNasm();		// push back and generate byte code
	int importObj();		// manual - taking name of func, byte code of func and creating new token
	
	void pushBack(int type, const char* text, int ivalue, float fvalue, char cvalue, const char* svalue);
	
	//----------- Just a covers for default pushBack -----------//
	void pushBack(const char* nasm_code);						//
	void pushBack(const char* nasm_code, int ivalue);			//
	void pushBack(const char* nasm_code, float fvalue);			//
	void pushBack(const char* nasm_code, char cvalue);			//
	void pushBack(const char* nasm_code, const char* svalue);	//
																//
	void pushBack(int type, const char* text);					//
	void pushBack(int type, const char* text, int ivalue);		//
	//----------------------------------------------------------//
	
	int exportNasm(const char* filename);
	int exportObj();
	int exportExecutable();
	int exportVirtualExecutable();
	
	/// rewrite /// rewrite /// rewrite //
	void makeNasm();					//
	void compileDef(ASN* node);			//
	void makeBody(ASN* node);			//
	void pushParameters(ASN* node);		//
	void compileNode(ASN* node);		//
	//////////////////////////////////////
};


class Binary::Token
{
	friend Binary;
	
  protected:
	int type = EMPTY;
	char* text = nullptr;
	int text_length = 0;
	
	int ivalue = 0;
	float fvalue = 0;
	char cvalue = 0;
	char* svalue = nullptr;

	long long int first_byte_position = 0;
	
	Token* prev = nullptr;
	Token* next = nullptr;
	
	enum TokenTypes
	{
		EMPTY, 
		LOCAL_LABEL, GLOBAL_LABEL, FUNCTION_LABEL,
		NASM_CODE, BYTE_CODE,
		TYPES_COUNT
	};
	
  public:
  	Token();
	void setText(const char* text);
	void setText(const char* bytes, long int bytes_count);
	void setSValue(const char* svalue);
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


//typedef Binary::Token Token;
//typedef Binary::Header Header;



