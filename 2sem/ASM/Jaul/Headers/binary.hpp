/*!
 *	@file binary.hpp
 *	@brief Описание класса бинарного кода
 */


#pragma once

#include <cstring>
#include <cassert>
#include "ast.hpp"
#include "source.hpp"

class Binary
{
  public:
  	class Token;
	
	Source* source = nullptr;
	
	Token* start = nullptr;
	Token* end = nullptr;
	
	Token** labels = nullptr;
	unsigned int labels_count = 0;
	
	Binary(Source* source);
	~Binary();
	
	void compile(bool verbose);
	
	//---- Abstract syntax tree compilation ----//
	void compileAst(bool verbose);				//
	void compileDef(ASN* node);					//
	void compileBody(ASN* node);				//
	void compileParameters(ASN* node);			//
	void compileNode(ASN* node);				//
	//------------------------------------------//
	
	void importNasm(bool verbose);		// push back and generate byte code
	void importObj(bool verbose);		// manual - taking name of func, byte code of func and creating new token
	
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
	
	int translate();
	
	int exportNasm(const char* filename);
	int exportObj(const char* filename);
	int exportExecutable(const char* filename);
	int exportVirtualExecutable(const char* filename);
	int exportHex(const char* filename);
};


class Binary::Token
{
	friend Binary;
	
  protected:
	int type = EMPTY;
	
	char* text = nullptr;
	int text_length = 0;
	
	char* bytes = nullptr;
	int bytes_count = 0;
	
	int ivalue = 0;
	float fvalue = 0;
	char cvalue = 0;
	char* svalue = nullptr;

	unsigned long long int first_byte_position = 0;
	
	Token* prev = nullptr;
	Token* next = nullptr;
	
	Binary* container = nullptr;
	
	//! Типы данных в токене
	enum TokenTypes
	{
		EMPTY, 			///< Пустой токен
		LOCAL_LABEL, 	///< Токен содержит локальную метку
		GLOBAL_LABEL, 	///< Токен содержит глобальную метку
		FUNCTION_LABEL,	///< Токен содержит метку функции
		NASM_CODE, 		///< Токен содержит ассемблерный код
		BYTE_CODE, 		///< Токен содержит скомпилированный код
		BOTH,			///< Токен содержит ассемблерный и скомпилированный код
		TYPES_COUNT		///< Количество типов токенов
	};
	
  public:
  	Token();
	void setText(const char* text);
	void setBytes(const char* bytes, long int bytes_count);
	void setSValue(const char* svalue);
	~Token();
	
	int decompile();	///< Декомпиляция из байт-кода в ассемблерный, если возможно
	int compile();		///< Компиляция из ассемблерного кода в байт-код
	int prepare();		///< Подготовка ассемблерной команды, приведение ее к обобщенному виду
};

//typedef Binary::Token Token;



