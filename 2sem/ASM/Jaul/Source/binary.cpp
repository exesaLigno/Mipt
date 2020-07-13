#include "../Headers/binary.hpp"

Binary::Binary(Source* source)
{
	this -> source = source;
}

Binary::~Binary()
{}



void Binary::compile(bool verbose)
{
	if (this -> source -> source_type == Source::JAUL_SOURCE)
		this -> compileAst(verbose);
		
	else if (this -> source -> source_type == Source::JASM_SOURCE)
		this -> importNasm(verbose);
		
	else if (this -> source -> source_type == Source::JAUL_OBJ)
		this -> importObj(verbose);
		
	else
		printf("\x1b[1;31merror:\x1b[0m trying to compile non-compilable source \x1b[1m<%s>\x1b[0m\n", this -> source -> name);
}



void Binary::compileAst(bool verbose)
{
	ASN* current_def = (this -> source -> ast).head -> left;
	
	while (current_def)
	{
		compileDef(current_def -> right);
		current_def = current_def -> left;
	}
	
	if (verbose)
		printf("\x1b[1m%s:\x1b[0m JAUL source compiled and imported to binary\n", this -> source -> name);
}

void Binary::compileDef(ASN* node)
{
	this -> pushBack(Token::FUNCTION_LABEL, node -> svalue);
	this -> pushBack("pop r13");
	this -> pushBack("mov r15, rsp");
	this -> pushBack("sub rsp, %d", (node -> ivalue) * 8);
	this -> pushBack("mov r14, rsp");
	
	compileBody(node -> left);
	
	this -> pushBack("add rsp, %d", (node -> ivalue) * 8);
	this -> pushBack("push r13");
	
	if (!strcmp(node -> svalue, "_start"))
	{
		this -> pushBack("mov rdi, rax");
		this -> pushBack("mov rax, 60");
		this -> pushBack("syscall");
	}
		
	else
		this -> pushBack("ret");
}

void Binary::compileBody(ASN* node)
{
	int type = node -> type;
	int optype = node -> ivalue;
	
	if (type == ASN::FUNCCALL)
	{
		this -> pushBack("push r13");
		this -> pushBack("push r14");
		this -> pushBack("push r15");
		
		if (node -> right)
			compileParameters(node -> right);

		this -> pushBack("call %s", node -> svalue);
		
		ASN* param = node -> right;
		
		while (param)
		{
			this -> pushBack("pop r15");
			param = param -> left;
		}
		
		this -> pushBack("pop r15");
		this -> pushBack("pop r14");
		this -> pushBack("pop r13");
		this -> pushBack("push rax");
	}
	
	else if (optype == ASN::WHILE and type == ASN::CTRL_OPERATOR)
	{
		this -> pushBack(Token::LOCAL_LABEL, ".cycle%d", node -> vartype);
		
		if (node -> right)
			compileBody(node -> right);
			
		this -> pushBack("pop rax");
		this -> pushBack("test rax, rax");
		this -> pushBack("jz .exitcycle%d", node -> vartype);
		
		if (node -> left)
			compileBody(node -> left);

		this -> pushBack("jmp .cycle%d", node -> vartype);
		
		this -> pushBack(Token::LOCAL_LABEL, ".exitcycle%d", node -> vartype);
	}
	
	else if (optype == ASN::FOR and type == ASN::CTRL_OPERATOR)
	{
		this -> pushBack("For compilation");
	}
	
	else if (optype == ASN::IF and type == ASN::CTRL_OPERATOR)
	{
		if (node -> right)
			compileBody(node -> right);

		this -> pushBack("pop rax");
		this -> pushBack("test rax, rax");
		this -> pushBack("jz .endif%d", node -> vartype);
		
		if (node -> left)
			compileBody(node -> left);

		this -> pushBack("jmp .exitif%d", node -> vartype);
		
		this -> pushBack(Token::LOCAL_LABEL, ".endif%d", node -> vartype);
		
		
		// Else compilation
		if (node -> parent -> left)
		{
			if (node -> parent -> left -> right -> type == ASN::CTRL_OPERATOR and node -> parent -> left -> right -> ivalue == ASN::ELSE)
				compileBody(node -> parent -> left -> right -> left);
		}

		this -> pushBack(Token::LOCAL_LABEL, ".exitif%d", node -> vartype);
	}
	
	else if (optype == ASN::ELSE and type == ASN::CTRL_OPERATOR)
		this -> pushBack("nop");
		
	else
	{
		if (node -> right)
			compileBody(node -> right);
		
		if (node -> left)
			compileBody(node -> left);
		
		compileNode(node);
	}
}

void Binary::compileParameters(ASN* node)
{
	if (node -> left)
		compileParameters(node -> left);
		
	if (node -> right)
		compileBody(node -> right);
}

void Binary::compileNode(ASN* node)
{
	if (node -> type == ASN::ARITHM_OPERATOR or node -> type == ASN::CMP_OPERATOR or node -> type == ASN::CTRL_OPERATOR)
	{
		#define TOKEN(string, token_type, token_number, dump, nasm_code, bin_code)		\
				case token_number:														\
				{																		\
					this -> pushBack(nasm_code);										\
					break;																\
				}	
		
		switch (node -> ivalue)
		{
			#include "../Syntax/jaul_syntax.hpp"
			
			default:
			{
				this -> pushBack("nop");
				break;
			}
		}
		
		#undef TOKEN
	}
	
	else if (node -> type == ASN::VARIABLE)
	{
		if (node -> LValue)
		{
			if (node -> vartype == ASN::LOCAL)
			{
				this -> pushBack("mov rax, r14");
				this -> pushBack("add rax, %d", (node -> ivalue) * 8);
				this -> pushBack("push rax");
			}
			else if (node -> vartype == ASN::PARAMETER)
			{
				this -> pushBack("mov rax, r15");
				this -> pushBack("add rax, %d", (node -> ivalue) * 8);
				this -> pushBack("push rax");
			}
		}
		else
		{
			if (node -> vartype == ASN::LOCAL)
			{
				this -> pushBack("mov rax, [r14 + %d]", (node -> ivalue) * 8);
				this -> pushBack("push rax");
			}
			else if (node -> vartype == ASN::PARAMETER)
			{
				this -> pushBack("mov rax, [r15 + %d]", (node -> ivalue) * 8);
				this -> pushBack("push rax");
			}
		}
	}
		
	else if (node -> type == ASN::INT)
	{
		this -> pushBack("mov rax, %d", node -> ivalue);
		this -> pushBack("push rax");
	}
		
	else if (node -> type == ASN::FLOAT)
	{
		this -> pushBack("mov rax, %f", node -> fvalue);
		this -> pushBack("push rax");
	}
	
	else if (node -> type == ASN::CHAR)
	{
		this -> pushBack("mov rax, \'%c\'", node -> cvalue);
		this -> pushBack("push rax");
	}
		
	else if (node -> type == ASN::STRING)
		this -> pushBack("String");
}



void Binary::importNasm(bool verbose)
{
	printf("\x1b[1;31mNASM import is not implemented yet!\x1b[0m\n");
	
	if (verbose)
		printf("\x1b[1m%s:\x1b[0m JASM code imported to binary\n", this -> source -> name);
}

void Binary::importObj(bool verbose)
{
	printf("\x1b[1;31mObjects import is not implemented yet!\x1b[0m\n");
	
	if (verbose)
		printf("\x1b[1m%s:\x1b[0m object imported to binary\n", this -> source -> name);
}



void Binary::pushBack(int type, const char* text, int ivalue, float fvalue, char cvalue, const char* svalue)
{
	Token* new_token = new Token;
	
	if (not this -> start)
	{
		this -> start = new_token;
		this -> end = new_token;
	}
	
	else
	{
		this -> end -> next = new_token;
		new_token -> prev = this -> end;
		this -> end = new_token;
	}
	
	new_token -> type = type;
	new_token -> setText(text);
	new_token -> ivalue = ivalue;
	new_token -> fvalue = fvalue;
	new_token -> cvalue = cvalue;
	new_token -> setSValue(svalue);
}

void Binary::pushBack(const char* nasm_code)
{
	int indent = 0;
	
	while (*(nasm_code + indent) != '\0')
	{
		if (*(nasm_code + indent) == '\n')
			indent++;
			
		const char* endline = strchr(nasm_code + indent, '\n');
		const char* endcode = strchr(nasm_code + indent, '\0');
		
		int length = 0;
		
		if (endline)
			length = endline - (nasm_code + indent);
		
		else
			length = endcode - (nasm_code + indent);
			
		char* nasm_token = new char[length + 1]{0};
		strncpy(nasm_token, nasm_code + indent, length);
		
		this -> pushBack(Token::NASM_CODE, nasm_token, 0, 0, 0, 0);
		
		delete[] nasm_token;
		
		indent += length;
	}
}

void Binary::pushBack(const char* nasm_code, int ivalue)
{
	this -> pushBack(Token::NASM_CODE, nasm_code, ivalue, 0, 0, 0);
}

void Binary::pushBack(const char* nasm_code, float fvalue)
{
	this -> pushBack(Token::NASM_CODE, nasm_code, 0, fvalue, 0, 0);
}

void Binary::pushBack(const char* nasm_code, char cvalue)
{
	this -> pushBack(Token::NASM_CODE, nasm_code, 0, 0, cvalue, 0);
}

void Binary::pushBack(const char* nasm_code, const char* svalue)
{
	this -> pushBack(Token::NASM_CODE, nasm_code, 0, 0, 0, svalue);
}

void Binary::pushBack(int type, const char* text)
{
	this -> pushBack(type, text, 0, 0, 0, 0);
}

void Binary::pushBack(int type, const char* text, int ivalue)
{
	this -> pushBack(type, text, ivalue, 0, 0, 0);
}



int Binary::exportNasm(const char* filename)
{
	FILE* export_file = fopen(filename, "a");

	Token* current = this -> start; // i love you
	
	while (current)
	{
		if (current -> type != Token::BYTE_CODE)
		{
			if (current -> type == Token::FUNCTION_LABEL)
				fprintf(export_file, "\n\n");
			
			if (current -> type == Token::NASM_CODE)
				fprintf(export_file, "\t");
				
			
			if (strstr(current -> text, "%d") != nullptr)
				fprintf(export_file, current -> text, current -> ivalue);
				
			else if (strstr(current -> text, "%f") != nullptr)
				fprintf(export_file, current -> text, current -> fvalue);
				
			else if (strstr(current -> text, "%c") != nullptr)
				fprintf(export_file, current -> text, current -> cvalue);
				
			else if (strstr(current -> text, "%s") != nullptr)
				fprintf(export_file, current -> text, current -> svalue);
			
			else
				fprintf(export_file, current -> text);
			
				
			if (current -> type == Token::LOCAL_LABEL or current -> type == Token::GLOBAL_LABEL or current -> type == Token::FUNCTION_LABEL)
				fprintf(export_file, ":\n");
				
			if (current -> type == Token::NASM_CODE)
				fprintf(export_file, "\n");
		}
		
		current = current -> next;
	}
	
	fclose(export_file);
	
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

Binary::Token::~Token()
{
	if (this -> text != nullptr)
		delete[] this -> text;
}



void Binary::Token::setText(const char* text)
{
	this -> text = new char[strlen(text) + 1]{0};
	strcpy(this -> text, text);
}

void Binary::Token::setText(const char* bytes, long int bytes_count)
{
	this -> text = new char[bytes_count]{0};
	strncpy(this -> text, bytes, bytes_count);
}

void Binary::Token::setSValue(const char* svalue)
{
	if (svalue)
	{
		this -> svalue = new char[strlen(svalue) + 1]{0};
		strcpy(this -> svalue, svalue);
	}
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


////////////////////////////////////////////////////////////////////////////////


