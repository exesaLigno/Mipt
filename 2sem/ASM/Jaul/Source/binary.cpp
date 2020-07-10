#include "../Headers/binary.hpp"

Binary::Binary()
{}

Binary::Binary(Source* source)
{
	this -> source = source;
}

Binary::~Binary()
{}

int Binary::compileAst()
{
	return 0;
}

int Binary::compileNasm()
{
	return 0;
}

int Binary::importObj()
{
	return 0;
}

int Binary::pushBack(const char* nasm_code)
{
	
	
	return 0;
}

int Binary::exportNasm()
{
	return 0;
}

int Binary::exportObj()
{
	return 0;
}

int Binary::exportExecutable()
{
	return 0;
}


////////////////////////////////////////////////////////////////////////////////


Binary::Token::Token()
{}

Binary::Token::Token(const char* name, const char* nasm_text)
{
	this -> label = new char[strlen(name) + 1]{0};
	strcpy(this -> label, name);
	
	this -> nasm_code = new char[strlen(name) + 1]{0};
	strcpy(this -> nasm_code, nasm_text);
}

Binary::Token::~Token()
{
	if (this -> label != nullptr)
		delete[] this -> label;
		
	if (this -> nasm_code != nullptr)
		delete[] this -> nasm_code;
		
	if (this -> byte_code != nullptr)
		delete[] this -> byte_code;
}

int Binary::Token::setLabel(const char* name, int name_length)
{
	this -> label = new char[name_length + 1]{0};
	strncpy(this -> label, name, name_length);
	return 0;
}

int Binary::Token::setBytes(const char* bytes, long int bytes_count)
{
	this -> byte_code = new char[bytes_count + 1]{0};
	strncpy(this -> byte_code, bytes, bytes_count);
	return 0;
}

int Binary::Token::toNasm()
{
	return 0;
}

int Binary::Token::toByte()
{
	return 0;
}


////////////////////////////////////////////////////////////////////////////////


Binary::Header::Header()
{}

Binary::Header::~Header()
{}
