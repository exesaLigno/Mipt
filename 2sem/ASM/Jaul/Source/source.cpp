/*!
 *	@file source.cpp
 *	@brief Исполняемый код класса Source
 */


#include "../Headers/source.hpp"



#define skip_spaces(where)	while (*where == ' ')	\
								(where)++;			\


Source::Source()
{
	this -> source_type = DEFINITION;
	this -> ast = new AST;
}

Source::Source(const char* name)
{
	this -> name = new char[strlen(name) + 1]{0};
	strcpy(this -> name, name);
	
	this -> source_type = this -> getType();
	
	if (this -> source_type == ERRTYPE)
		this -> status = UNSUPPORTED_FILE_EXTENSION;
	
	this -> ast = new AST;
}

Source::~Source()
{
	if (this -> name != nullptr)
		delete[] this -> name;
		
	if (this -> text != nullptr)
		delete[] this -> text;
	
	if (this -> ast != nullptr)
		delete this -> ast;
}

bool Source::open()
{
	if (this -> source_type == DEFINITION)
	{
		this -> status = OPENNING_PROHIBITED;
		return false;
	}
		
	FILE* source_file = fopen(this -> name, "r");	// Trying to open file
	
	if (not source_file)	// Trying to find file in std library path
	{
		// TODO config reading from Jaul.conf
		char* std_name = new char[strlen(this -> name) + strlen(".std/") + 1]{0};
		strcpy(std_name, ".std/");
		strcat(std_name, this -> name);
		source_file = fopen(std_name, "r");
		delete[] std_name;
	}
	
	if (not source_file)
	{
		printf("\x1b[1;31merror\x1b[0m: file \x1b[1m\'%s\'\x1b[0m not exist\n", this -> name);
		this -> status = FILE_NOT_EXIST;
		return false;
	}
	
	fseek(source_file, 0, SEEK_END);
	this -> text_length = ftell(source_file);
	rewind(source_file);

	this -> text = new char[this -> text_length + 1]{0};
	
	this -> text_pointer = this -> text;

	this -> text_length = fread(this -> text, sizeof(char), this -> text_length, source_file);
	
	fclose(source_file);
	
	return true;
}

short int Source::getType()
{
	const char* extension = strrchr(name, '.');
	
	if (!strcmp(extension, ".j"))
		return JAUL_SOURCE;
		
	else if (!strcmp(extension, ".s"))
		return JASM_SOURCE;
		
	else if (!strcmp(extension, ".jo"))
		return JAUL_OBJ;
		
	else if (!strcmp(extension, ".jv"))
		return VIRTUAL_EXECUTABLE;
		
	else
		return ERRTYPE;
}


void Source::print()
{
	if (this -> name)
		printf("\n\x1b[1;32m%s\x1b[0m (type = %d)\n\n", this -> name, this -> source_type);
		
	if (this -> text)
		printf("%s\n", this -> text);
	else 
		printf("\x1b[2mEmpty\x1b[0m\n");
}



/*----------------------\
| This fragment			|
| need to rewrited		|
| with new logic		|
\----------------------*/
void Source::makeAST()	
{
	
	if (this -> source_type != JAUL_SOURCE)
		return;
	
	int initial_indent = 0;
	
	ASN* entry = ast -> createNode(ASN::ENTRY);
	
	ASN* result = parseBlock(initial_indent, &(this -> text_pointer));
	
	entry -> leftConnect(result);
}


void Source::optimizeAST()
{
	int change_count = 0;
	change_count += inlineFunctions(this -> ast -> head);
	change_count += foldConstants(this -> ast -> head);
	change_count += substituteStatic(this -> ast -> head);
	change_count += foldConstants(this -> ast -> head);
	
	if (change_count != 0)
		this -> optimizeAST();
}


int Source::substituteStatic(ASN* node)
{
	int change_count = 0;
	return change_count;
}


int Source::foldConstants(ASN* node)
{
	int change_count = 0;
	
	if (node -> left)
		change_count += foldConstants(node -> left);
	
	if (node -> right)
		change_count += foldConstants(node -> right);
	
	if (node -> type == ASN::ARITHM_OPERATOR and node -> left and node -> right)
		change_count += foldArithmeticConstants(node);
	
	else if (node -> type == ASN::CMP_OPERATOR and node -> left and node -> right)
		change_count += foldCmpConstants(node);
	
	else if (node -> type == ASN::CTRL_OPERATOR and node -> left and node -> right)
		change_count += foldCtrlConstants(node);
	
	return change_count;
}


int Source::foldArithmeticConstants(ASN* node)
{
	int change_count = 0;
	
	switch (node -> ivalue)
	{
		case ASN::PLUS:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue + node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
				
				else if (node -> left -> type == ASN::INT and node -> left -> ivalue == 0)
				{
					ASN* right = node -> right;
					node -> right = nullptr;
					
					if (node -> parent -> left == node)
						node -> parent -> leftConnect(right);
					
					else if (node -> parent -> right == node)
						node -> parent -> rightConnect(right);
					
					delete node;
					
					change_count++;
				}

				else if (node -> right -> type == ASN::INT and node -> right -> ivalue == 0)
				{
					ASN* left = node -> left;
					node -> left = nullptr;
					
					if (node -> parent -> left == node)
						node -> parent -> leftConnect(left);
					
					else if (node -> parent -> right == node)
						node -> parent -> rightConnect(left);
					
					delete node;
					
					change_count++;
				}
			}
		
			break;
		}
		
		case ASN::MINUS:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue - node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
				
// 				else if (node -> left -> type == ASN::INT and node -> left -> ivalue == 0)	///< unary "-" with "neg" command working incorrect
// 				{
// 					node -> left = node -> right;
// 					node -> right = nullptr;
// 					
// 					node -> ivalue = ASN::UNARY_MINUS;
// 				}
				
				else if (node -> right -> type == ASN::INT and node -> right -> ivalue == 0)
				{
					ASN* left = node -> left;
					node -> left = nullptr;
					
					if (node -> parent -> left == node)
						node -> parent -> leftConnect(left);
					
					else if (node -> parent -> right == node)
						node -> parent -> rightConnect(left);
					
					delete node;
					
					change_count++;
				}
			}
			
			break;
		}
		
		case ASN::MULTIPLY:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue * node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
				
				else if (node -> left -> type == ASN::INT and node -> left -> ivalue == 1)
				{
					ASN* right = node -> right;
					node -> right = nullptr;
					
					if (node -> parent -> left == node)
						node -> parent -> leftConnect(right);
					
					else if (node -> parent -> right == node)
						node -> parent -> rightConnect(right);
					
					delete node;
					
					change_count++;
				}

				else if (node -> right -> type == ASN::INT and node -> right -> ivalue == 1)
				{
					ASN* left = node -> left;
					node -> left = nullptr;
					
					if (node -> parent -> left == node)
						node -> parent -> leftConnect(left);
					
					else if (node -> parent -> right == node)
						node -> parent -> rightConnect(left);
					
					delete node;
					
					change_count++;
				}
				
				else if ((node -> right -> type == ASN::INT and node -> right -> ivalue == 0) or (node -> left -> type == ASN::INT and node -> left -> ivalue == 0))
				{
					delete node -> right;
					node -> right = nullptr;
					
					delete node -> left;
					node -> left = nullptr;
					
					node -> type = ASN::INT;
					node -> ivalue = 0;
					
					change_count++;
				}
			}
			
			break;
		}
	}
	
	return change_count;
}


int Source::foldCmpConstants(ASN* node)
{
	int change_count = 0;
	
	switch (node -> ivalue)
	{
		case ASN::EQUAL:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue == node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
			
			break;
		}
		
		case ASN::NOT_EQUAL:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue != node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
			
			break;
		}
		
		case ASN::MORE:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue > node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
			
			break;
		}
		
		case ASN::MORE_EQ:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue >= node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
			
			break;
		}
		
		case ASN::LESS:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue < node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
			
			break;
		}
		
		case ASN::LESS_EQ:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue <= node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
			
			break;
		}
		
		case ASN::AND:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue and node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
				
				else if (node -> left -> type == ASN::INT and node -> left -> ivalue == 0)
				{
					node -> type = ASN::INT;
					node -> ivalue = 0;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
				
				else if (node -> right -> type == ASN::INT and node -> right -> ivalue == 0)
				{
					node -> type = ASN::INT;
					node -> ivalue = 0;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
						
			break;
		}
		
		case ASN::OR:
		{
			if (node -> left and node -> right)
			{
				if (node -> left -> type == ASN::INT and node -> right -> type == ASN::INT)
				{
					node -> type = ASN::INT;
					node -> ivalue = node -> left -> ivalue or node -> right -> ivalue;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
				
				else if (node -> left -> type == ASN::INT and node -> left -> ivalue == 1)
				{
					node -> type = ASN::INT;
					node -> ivalue = 1;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
				
				else if (node -> right -> type == ASN::INT and node -> right -> ivalue == 1)
				{
					node -> type = ASN::INT;
					node -> ivalue = 1;
					
					delete node -> left;
					node -> left = nullptr;
					
					delete node -> right;
					node -> right = nullptr;
					
					change_count++;
				}
			}
						
			break;
		}
	}
	
	return change_count;
}


int Source::foldCtrlConstants(ASN* node) ///< TODO Valgrind maybe
{
	int change_count = 0;
	
	switch (node -> ivalue)
	{
		case ASN::IF:
		{
			if (node -> right -> type == ASN::INT and node -> right -> ivalue == 0)
			{
				node -> parent -> parent -> leftConnect(node -> parent -> left);
				ASN* next_line = node -> parent -> left;
				
				node -> parent -> left = nullptr;
				delete node -> parent;
				
				if (next_line)
				{				
					if (next_line -> right -> type == ASN::CTRL_OPERATOR and next_line -> right -> ivalue == ASN::ELSE)
					{
						next_line -> parent -> leftConnect(next_line -> right -> left);
						
						ASN* line = next_line -> right -> left;
						if (line)
						{
							while (line -> left)
								line = line -> left;
						}
						
						line -> leftConnect(next_line -> left);
						
						next_line -> left = nullptr;
						next_line -> right -> left = nullptr;
						
						delete next_line;
					}
				}
				
				change_count++;
			}
			
			else if (node -> right -> type == ASN::INT and node -> right -> ivalue != 0)
			{
				node -> parent -> parent -> leftConnect(node -> left);
				
				ASN* line = node -> left;
				
				node -> left = nullptr;
				
				if (line)
				{
					while (line -> left)
						line = line -> left;
				}
				
				line -> leftConnect(node -> parent -> left);
				
				ASN* next_line = node -> parent -> left;
				
				node -> parent -> left = nullptr;
				
				delete node -> parent;
				
				if (next_line)
				{
					if (next_line -> right -> type == ASN::CTRL_OPERATOR and next_line -> right -> ivalue == ASN::ELSE)
					{
						next_line -> parent -> leftConnect(next_line -> left);
						next_line -> left = nullptr;
						
						delete next_line;
					}
				}
				
				change_count++;
			}
			
			break;
		}
		
		case ASN::WHILE:
		{
			if (node -> right -> type == ASN::INT and node -> right -> ivalue == 0)
			{
				node -> parent -> parent -> leftConnect(node -> parent -> left);
				node -> parent -> left = nullptr;
				delete node -> parent;
				
				change_count++;
			}
			
			break;
		}
	}
	
	return change_count;
}



int Source::inlineFunctions(ASN* node)
{
	int change_count = 0;
	return change_count;
}




void Source::prepareAST()
{
	this -> splitFunctions();
	this -> enumerateMembers();
}


int Source::calculateIndent(char* text)
{
	int indent = 0;
	
	while (*text == '\t')
	{
		indent++;
		text++;
	}
		
	return indent;
}


bool Source::nextLine(char** _text)
{
	while (**_text != '\0')
	{		
		if (**_text == '\n')
		{
			(*_text)++;
			(this -> current_line)++;
		}
			
		char* line_start = *_text;
		
		bool empty = true;
		
		while (**_text != '\n' and **_text != '\0')
		{
			if (**_text != '\t' and **_text != ' ')
			{
				empty = false;
				break;
			}
			
			(*_text)++;
		}
		
		if (not empty)
		{
			*_text = line_start;
			break;
		}
	}
	

	//char* 
	
	if (**_text == '\0')
		return false;
		
	else
		return true;
}


ASN* Source::parseBlock(int indent, char** _text)
{
	ASN* head = 0;
	ASN* previous = 0;
	
	while (**_text != '\0')
	{
		bool line_founded = nextLine(_text);
		
		int current_indent = calculateIndent(*_text);
		
		if (current_indent > indent)
			printf("\x1b[1;31mIncorrect indent!\x1b[0m\n");

		if (current_indent < indent)
			break;
		
		if (line_founded)
		{
			ASN* newline = ast -> createNode(ASN::LINE);
			newline -> ivalue = this -> current_line;
			newline -> svalue = new char[strlen(this -> name) + 1];
			strcpy(newline -> svalue, this -> name);
			
			if (previous)
			{
				previous -> leftConnect(newline);
				previous = newline;
			}
			
			else
			{
				previous = newline;
				head = newline;
			}
			
			ASN* parsed = parseLine(indent, _text); // here segfault
			
			newline -> rightConnect(parsed);
		}
	}
	
	return head;
}


ASN* Source::parseLine(int indent, char** _text)
{
	while (**_text == '\t')
		(*_text)++;
	
	char* endline = strchr(*_text, '\n');
	if (endline == NULL)
		endline = strchr(*_text, '\0');
		
	int length = endline - *_text;

	char* line = new char[length + 1]{0};
	strncpy(line, *_text, length);
	
	char* _line = line;
	
	ASN* node = getInclude(indent, &_line);
	
	(*_text) += length;
	
	if ((node -> type == ASN::CTRL_OPERATOR and (node -> ivalue == ASN::WHILE or node -> ivalue == ASN::IF or node -> ivalue == ASN::ELSE)) or node -> type == ASN::FUNC)
	{
		ASN* internal = parseBlock(indent + 1, _text);
		node -> leftConnect(internal);
	}
	
	if (line)
		delete[] line;
	
	
	return node;
}


ASN* Source::getInclude(int indent, char** _line)
{
	
	if (!strncmp(*_line, "include ", 8))
	{
		
		*_line += 8;
		ASN* include = new ASN(ASN::INCLUDE, _line);
		
		char* source_prefix_end = strrchr(this -> name, '/');
		if (source_prefix_end != nullptr)
		{
			int source_prefix_length = source_prefix_end - this -> name + 1;
			char* fixed_name = new char[source_prefix_length + strlen(include -> svalue) + 1]{0};
			strncpy(fixed_name, this -> name, source_prefix_length);
			strcat(fixed_name, include -> svalue);
			
			delete[] include -> svalue;
			include -> svalue = fixed_name;
		}
		
		if (strrchr(include -> svalue, '.') == nullptr)	// TODO add searching algorithm
		{
			char* jaul_library_name = new char[strlen(include -> svalue) + 3];
			sprintf(jaul_library_name, "%s.j", include -> svalue);			
			
			delete[] include -> svalue;
			include -> svalue = jaul_library_name;
		}
		
		Source library(include -> svalue);
		library.open();
		
		if (library.status != OK)
		{
			printf("%s:%d: \x1b[1;31merror:\x1b[0m including file \x1b[1m\"%s\"\x1b[0m not exist.\n", this -> name, this -> current_line, library.name);
			return include;
		}
			
		else
		{
			library.makeAST();
			include -> leftConnect(library.ast -> head -> left);
			include -> rightConnect(library.ast -> head -> right);
			library.ast -> head -> left = 0;
			library.ast -> head -> right = 0;
		}
		
		return include;
	}
	
	else
	{
		
		return getDef(indent, _line);
		
	}
	
}


ASN* Source::getDef(int indent, char** _line)
{
	if (!strncmp(*_line, "def ", 4))
	{
		(*_line) += 4;
		
		ASN* def = ast -> createNode(ASN::FUNC, _line);
		
		(*_line)++;
		
		ASN* itemize = getItemize(indent, _line);
		
		def -> rightConnect(itemize);
		
		return def;
	}
	
	else
	{
		
		return getAssignment(indent, _line);
		
	}
}


ASN* Source::getAssignment(int indent, char** _line)	// get assignment
{	
	skip_spaces(*_line);
	ASN* left_branch = getOperators(indent, _line);
	
	skip_spaces(*_line);
	if (**_line == '=' and *(*_line + 1) != '=')
	{		
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getOperators(indent, _line);
			
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getOperators(int indent, char** _line)
{	
	ASN* left_branch = 0;

	if (!strncmp(*_line, "while", 5) or !strncmp(*_line, "if", 2) or !strncmp(*_line, "for", 4) or !strncmp(*_line, "return", 6))
	{
		
		ASN* operand = ast -> createNode(_line);
		
		skip_spaces(*_line);
		ASN* right_branch = getLogic(indent, _line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	else if (!strncmp(*_line, "else", 4))
	{
		
		ASN* operand = ast -> createNode(_line);
		
		skip_spaces(*_line);
		left_branch = operand;		
	}
	
	else
	{
		
		skip_spaces(*_line);
		left_branch = getLogic(indent, _line);
		
		skip_spaces(*_line);
		
	}
	
	return left_branch;
}


ASN* Source::getLogic(int indent, char** _line)
{
	skip_spaces(*_line);
	
	ASN* left_branch = getCmp(indent, _line);
	
	
	skip_spaces(*_line);
	while (!strncmp(*_line, "||", 2) or !strncmp(*_line, "&&", 2))
	{		
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getCmp(indent, _line);
			
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getCmp(int indent, char** _line)
{
	
	skip_spaces(*_line);
	ASN* left_branch = getAddSub(indent, _line);
	
	skip_spaces(*_line);
	while (!strncmp(*_line, "==", 2) or !strncmp(*_line, "!=", 2) or 
		!strncmp(*_line, ">=", 2) or !strncmp(*_line, "<=", 2) or
		!strncmp(*_line, ">", 1) or !strncmp(*_line, "<", 1))
	{
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getAddSub(indent, _line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getAddSub(int indent, char** _line)
{
	
	skip_spaces(*_line);
	ASN* left_branch = getMulDiv(indent, _line);
	
	skip_spaces(*_line);
	
	while (**_line == '+' or **_line == '-')
	{
		if (not left_branch)
		{
			left_branch = ast -> createNode(ASN::INT);
			left_branch -> ivalue = 0;
		}
		
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getMulDiv(indent, _line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getMulDiv(int indent, char** _line)
{
	
	skip_spaces(*_line);
	ASN* left_branch = getPow(indent, _line);
	
	skip_spaces(*_line);
	while (**_line == '*' or **_line == '/' or **_line == '%')
	{
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getPow(indent, _line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getPow(int indent, char** _line)
{
	
	skip_spaces(*_line);
	ASN* left_branch = getNumVarFunc(indent, _line);
	
	skip_spaces(*_line);
	if (**_line == '^')
	{
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getNumVarFunc(indent, _line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}

	
ASN* Source::getNumVarFunc(int indent, char** _line)	// get numbers, variables and functions
{
	
	skip_spaces(*_line);
	
	if (**_line == '(')
	{
		(*_line)++;
		
		ASN* block = getLogic(indent, _line);
		
		if (**_line != ')')
		{
			char operand[20] = {};
			for (int i = 0; strchr(" \n\t\0", (*_line)[i]) == nullptr; i++)
				operand[i] = (*_line)[i];
			printf("\x1b[1;31merror:\x1b[0m <%s:%d>: enclosing bracket expected, but \x1b[1m\'%s\'\x1b[0m met\n", this -> name, this -> current_line, operand);
			this -> status = SYNTAX_ERROR;
		}
		
		(*_line)++;
		return block;
	}
	
	skip_spaces(*_line);
	
	if (**_line == '-')
		return 0;
	
	ASN* parsed = ast -> createNode(_line);
	
	if (parsed == nullptr)
	{
		printf("\x1b[1;31merror:\x1b[0m <%s:%d>: supposed operand, but endline met\n", this -> name, this -> current_line);
		this -> status = SYNTAX_ERROR;
	}
	
	else if (parsed -> type != ASN::FUNCCALL and parsed -> type != ASN::VARIABLE and parsed -> type != ASN::INT and parsed -> type != ASN::FLOAT)
	{
		printf("\x1b[1;31merror:\x1b[0m <%s:%d>: supposed operand, but operator ", this -> name, this -> current_line);
		parsed -> print();
		printf(" met\n");
		this -> status = SYNTAX_ERROR;
	}
	
	skip_spaces(*_line);
	if (**_line == '(')
	{
		(*_line)++;
		ASN* itemize = getItemize(indent, _line);
		
		if (itemize)
			parsed -> rightConnect(itemize);
		
		if (**_line != ')')
		{
			char operand[20] = {};
			for (int i = 0; strchr(" \n\t\0", (*_line)[i]) == nullptr; i++)
				operand[i] = (*_line)[i];
			printf("\x1b[1;31merror:\x1b[0m <%s:%d>: enclosing bracket expected, but \x1b[1m\'%s\'\x1b[0m met\n", this -> name, this -> current_line, operand);
			this -> status = SYNTAX_ERROR;
		}
		
		(*_line)++;
	}
	
	return parsed;
}


ASN* Source::getItemize(int indent, char** _line)
{
	
	skip_spaces(*_line);
	if (**_line == ')')
		return 0;
	
	ASN* item = ast -> createNode(ASN::ITEM);
	
	item -> rightConnect(getLogic(indent, _line));
	
	skip_spaces(*_line);
	
	if (**_line == ',')
	{
		
		(*_line)++;
		skip_spaces(*_line);
		item -> leftConnect(getItemize(indent, _line));
		
	}
	
	return item;
}


void Source::splitFunctions()
{
	if (this -> source_type != JAUL_SOURCE)
		return;
	
	ASN* last_function = 0;

	ASN* line = this -> ast -> head -> left;
	
	while (line)
	{
		if (line -> right -> type == ASN::FUNC)
		{
			ASN* def = line;
			def -> type = ASN::DEF;
			
			(line -> parent) -> leftConnect(line -> left);
			line = line -> left;
			
			def -> left = 0;
			
			if (last_function)
				last_function -> leftConnect(def);
				
			else
				this -> ast -> head -> rightConnect(def);
			
			last_function = def;
		}
		
		
		else if (line -> right -> type == ASN::INCLUDE)
		{
			ASN* deleting = line;
			
			ASN* new_lines = line -> right -> left;
			ASN* new_functions = line -> right -> right;
			line -> right -> left = 0;
			line -> right -> right = 0;
			
			line -> parent -> leftConnect(new_lines);
			
			if (last_function)
				last_function -> leftConnect(new_functions);
				
			else
			{
				this -> ast -> head -> rightConnect(new_functions);
				last_function = new_functions;
			}
			
			if (last_function)
			{
				while (last_function -> left)
					last_function = last_function -> left;
			}
			
			if (new_lines)
			{
				line = new_lines;
				while (line -> left)
					line = line -> left;
				
				line -> leftConnect(deleting -> left);
			}
			
			else
				deleting -> parent -> leftConnect(deleting -> left);
				
			deleting -> left = 0;
			delete deleting;
			
			line = new_lines;
		}
		
		else
			line = line -> left;
	}
	
	ASN* tmp = this -> ast -> head -> left;
	this -> ast -> head -> left = this -> ast -> head -> right;
	this -> ast -> head -> right = tmp;
	
	if (this -> ast -> head -> right)
	{
		ASN* _start = this -> ast -> head -> right;
		this -> ast -> head -> right = 0;
		
		ASN* _start_function_node = this -> ast -> createNode(ASN::_START);
		ASN* _start_def_node = this -> ast -> createNode(ASN::DEF);
		
		_start_def_node -> leftConnect(this -> ast -> head -> left);
		_start_def_node -> rightConnect(_start_function_node);
		_start_function_node -> leftConnect(_start);
		
		this -> ast -> head -> leftConnect(_start_def_node);
	}
}


void Source::enumerateMembers()
{
	ASN* def = this -> ast -> head -> left;
	
	while (def)
	{
		ASN* function = def -> right;
		
		ASN* parameter = function -> right;		
		
		while (parameter)
		{
			setType(function, parameter -> right -> svalue, ASN::PARAMETER);
			parameter = parameter -> left;
		}
		
		int varcounter = 0;
		while (true)
		{
			char* unnumerated_variable = getUnnumeratedVariable(function, ASN::LOCAL);
			if (not unnumerated_variable)
				break;
			
			setVariables(function, unnumerated_variable, varcounter);
			
			varcounter++;
		}
		
		function -> ivalue = varcounter;
		
		varcounter++;
		
		parameter = function -> right;
		
		while (parameter)
		{			
			setVariables(function, parameter -> right -> svalue, varcounter);
			parameter = parameter -> left;
			varcounter++;
		}
		
		def = def -> left;
	}
	
	int number = 0;
	enumerateBranching(this -> ast -> head, &number);
}



void Source::enumerateBranching(ASN* node, int* number)
{
	if (node -> type == ASN::CTRL_OPERATOR and (node -> ivalue == ASN::IF or node -> ivalue == ASN::WHILE or node -> ivalue == ASN::FOR))
	{
		node -> vartype = *number;
		(*number)++;
	}
	
	if (node -> right)
		enumerateBranching(node -> right, number);
		
	if (node -> left)
		enumerateBranching(node -> left, number);
}



void Source::setType(ASN* node, const char* varname, int vartype)
{
	if (varname == nullptr)
		return;
	
	if (node -> type == ASN::VARIABLE and node -> svalue)
	{
		if (!strcmp(node -> svalue, varname))
		{
			node -> vartype = vartype;
		}
	}
	
	if (node -> right)
		setType(node -> right, varname, vartype);
	
	if (node -> left)
		setType(node -> left, varname, vartype);
	
	return;
}



char* Source::getUnnumeratedVariable(ASN* node, int vartype)
{
	char* unnumerated_variable = nullptr;
	
	if (node -> type == ASN::VARIABLE and node -> ivalue == 0 and node -> vartype == vartype and (not node -> enumerated))
	{
		unnumerated_variable = node -> svalue;
	}
		
	if (node -> right and not unnumerated_variable)
		unnumerated_variable = getUnnumeratedVariable(node -> right, vartype);
		
	if (node -> left and not unnumerated_variable)
		unnumerated_variable = getUnnumeratedVariable(node -> left, vartype);	
		
	return unnumerated_variable;
}



void Source::setVariables(ASN* node, const char* varname, int varnumber)
{
	if (varname == nullptr)
		return;
	
	if (node -> type == ASN::VARIABLE and node -> svalue)
	{
		if (!strcmp(node -> svalue, varname))
		{
			node -> ivalue = varnumber;
			node -> enumerated = true;
		}
	}
	
	if (node -> right)
		setVariables(node -> right, varname, varnumber);
	
	if (node -> left)
		setVariables(node -> left, varname, varnumber);
	
	return;
}


void Source::dumpAST()
{
	ast -> dumper(this -> name, AST::DELETE_TXT);
	printf("\x1b[1m%s:\x1b[0m Programm tree dump saved as \x1b[1;32m<%s.png>\x1b[0m\n", this -> name, this -> name);
}



