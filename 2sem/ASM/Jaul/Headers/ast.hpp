/*!
 *	@file ast.hpp
 *	@brief Описание класса абстрактного синтаксического дерева
 */


#pragma once

#include <fstream>
#include <iostream>
#include <cstring>
#include "debug.hpp"


class AbstractSyntaxTree
{
  public:
  
	enum Modes
	{
		DEFAULT = 0,
		SHOW = 1,
		DELETE_PNG = 10,
		DELETE_TXT = 100,
		DETAILED = 1000
	};

	class Node
	{		
		friend class AbstractSyntaxTree;
		
	  public:
	  
		int type = 0;
	  	int ivalue = 0;
	  	float fvalue = 0;
	  	char cvalue = 0;
	  	char* svalue = nullptr;
	  	bool LValue = false;
	  	int vartype = 0;
	  	
	  	enum SYMTYPES
		{
			LETTER = 1, NUMBER, SYMBOL, SPECSYMBOL,
			COUNT_OF_SYMTYPES
		};
	  	
	  	enum VARTYPES
		{
			LOCAL = 1,
			PARAMETER
		};
	  	
	  	enum TYPES
	  	{
	  		UNKNOWN,
	  		ARITHM_OPERATOR, CMP_OPERATOR, CTRL_OPERATOR,		// OPERATORS
	  		VARIABLE, FUNCCALL, INT, FLOAT, STRING, CHAR,		// OPERANDS
	  		LINE, FUNC, ENTRY, ITEM, DEF,						// SPECIAL SYMBOLS
	  		_START, INCLUDE, DEFINE,
	  		COUNT_OF_TYPES
	  	};
	  	
	  	enum OPERATORS
	  	{
	  		NOP,
	  		PLUS, MINUS, MULTIPLY, DIVIDE, POWER,				// ariphmetics operators
	  		MODULO, INT_DIVISION, UNARY_MINUS,
	  		
	  		EQUAL, NOT_EQUAL, MORE, LESS, MORE_EQ, LESS_EQ,		// comparison operators
	  		AND, OR,
	  		
	  		IF, ELSE, WHILE, FOR, RETURN, ASSIGNMENT,			// control operators
	  		PLUS_ASSIGNMENT, MINUS_ASSIGNMENT,
	  		MULTIPLY_ASSIGNMENT, DIVIDE_ASSIGNMENT,
	  		POWER_ASSIGNMENT,
	  		
	  		COUNT_OF_OPERATORS
	  	};
		
		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;
	
		Node(int type);
  		Node(int type, char** text);
		Node(char** text);
		Node(const Node& that) = delete;
		~Node();
		
		void flush();
		
		void leftConnect(Node* left);
		void leftConnect(AbstractSyntaxTree* left);
		
		void rightConnect(Node* right);
		void rightConnect(AbstractSyntaxTree* right);
		
		void fixContainer();
		
		int dumper(std::ofstream& file, int mode);
		
		const char* colorize();
		void write(std::ofstream& out);
		
	  private:
		AbstractSyntaxTree* container = nullptr;
	};
	
	AbstractSyntaxTree();
	~AbstractSyntaxTree();
	
	int dumper(const char* filename, int mode);

	Node* createNode(int type);
	Node* createNode(int type, char** text);
	Node* createNode(char** text);
	
	Node* head = nullptr;
	
  private:
	unsigned long long int nodes_count = 0;	
};

typedef AbstractSyntaxTree AST;
typedef AST::Node ASN;


bool isLetter(char symbol);
bool isNumber(char symbol);
bool isOperand(char symbol);
bool isEnd(char symbol);

bool isInteger(char* string);
bool isFloat(char* string);
bool isChar(char* string);
bool isString(char* string);
