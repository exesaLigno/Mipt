#include <cstring>
#include <fstream>
#include "ast.hpp"

typedef AbstractSyntaxTree AST;
typedef AST::Node ASN;

class Source
{
  protected:
	char* name;
	short int source_type;
	char* text;
	long long int text_length;
	AbstractSyntaxTree ast;
	
  private:
	char* text_pointer;
	
  public:
	Source();
	Source(const char* name);
	~Source();
	
	short int open();
	short int getType(const char* name);
	void print();
	
	void makeAST();
	
	enum SourceTypes
	{
		JAUL_SOURCE = 1,		// .jaul
		NASM_SOURCE,			// .s
		OBJ_SOURCE,				// no extension
		VIRTUAL_EXECUTABLE,		// .jaulv
		DEFINITION,				// fileless type
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

