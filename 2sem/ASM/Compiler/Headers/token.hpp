#pragma once


class Token
{
  private:
  	int type;
  	int ivalue;
  	float fvalue;
  	char cvalue;
  	char* svalue;
  	
  	
  public:
  	enum TYPES
  	{
  		OPERATOR, NAME,				// CODE
  		INT, FLOAT, STRING, CHAR,	// OPERANDS
  		NEWLINE, NEWDEF, ENTRY,		// SPECIAL SYMBOLS
  		COUNT_OF_TYPES
  	};
  	Token();
  	Token(char* text);
  	Token(int type, int value);
  	explicit Token(const Token&);
  	~Token();
};
