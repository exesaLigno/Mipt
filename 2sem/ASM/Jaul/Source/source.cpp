/*!
 *	@file source.cpp
 *	@brief Исполняемый код класса Source
 */


#include "../Headers/source.hpp"



#define skip_spaces(where)	while (*where == ' ')	\
								(where)++;			\


Source::Source()
{
	this -> name = NULL;
	this -> text = NULL;
	this -> text_length = 0;
	this -> text_pointer = NULL;
	this -> source_type = DEFINITION;
	this -> ast = new AST;
}

Source::Source(const char* name)
{
	this -> name = new char[strlen(name) + 1]{0};
	strcpy(this -> name, name);
	
	this -> text = NULL;
	this -> text_length = 0;
	
	this -> text_pointer = NULL;
	
	this -> source_type = this -> getType();
	
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

short int Source::open()
{
	if (this -> source_type == DEFINITION)
		return OPENNING_PROHIBITED;
		
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
		//printf("\x1b[1;31mError\x1b[0m: file \x1b[1m\"%s\"\x1b[0m not exist.\n", this -> name);
		return FILE_NOT_EXIST;
	}
	
	fseek(source_file, 0, SEEK_END);
	this -> text_length = ftell(source_file);
	rewind(source_file);

	this -> text = new char[this -> text_length + 1]{0};
	
	this -> text_pointer = this -> text;

	this -> text_length = fread(this -> text, sizeof(char), this -> text_length, source_file);
	
	fclose(source_file);
	
	return OK;
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
	this -> rebuildTree();
	this -> prepareAST();
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
			
			ASN* parsed = parseLine(indent, _text);
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
	
	ASN* node = 0;
	
	if (!strncmp(line, "def ", 4))
		node = getDef(&_line);
		
	else if (!strncmp(line, "define ", 7))
		printf("defines not implemented yet!\n");
		
	else if (!strncmp(line, "include ", 8))
		node = getInclude(&_line);
	
	else
		node = getAssignment(&_line);

	
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


ASN* Source::getInclude(char** _line)
{
	*_line += 8;
	ASN* include = new ASN(ASN::INCLUDE, _line);
	Source library(include -> svalue);
	short int resp = library.open();
	
	if (resp != OK)
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


ASN* Source::getDef(char** _line)
{
	(*_line) += 4;
	
	ASN* def = ast -> createNode(ASN::FUNC, _line);
	
	(*_line)++;
	def -> rightConnect(getItemize(_line));
	
	return def;
}


ASN* Source::getAssignment(char** _line)	// get assignment
{
	skip_spaces(*_line);
	ASN* left_branch = getOperators(_line);
	
	skip_spaces(*_line);
	if (**_line == '=' and *(*_line + 1) != '=')
	{		
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getOperators(_line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getOperators(char** _line)
{
	ASN* left_branch = 0;

	if (!strncmp(*_line, "while", 5) or !strncmp(*_line, "if", 2) or !strncmp(*_line, "for", 4) or !strncmp(*_line, "return", 6))
	{
		ASN* operand = ast -> createNode(_line);
		
		skip_spaces(*_line);
		ASN* right_branch = getLogic(_line);
		
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
		left_branch = getLogic(_line);
		
		skip_spaces(*_line);
	}
	
	return left_branch;
}


ASN* Source::getLogic(char** _line)
{
	skip_spaces(*_line);
	ASN* left_branch = getCmp(_line);
	
	skip_spaces(*_line);
	while (!strncmp(*_line, "||", 2) or !strncmp(*_line, "&&", 2))
	{		
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getCmp(_line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getCmp(char** _line)
{
	skip_spaces(*_line);
	ASN* left_branch = getAddSub(_line);
	
	skip_spaces(*_line);
	while (!strncmp(*_line, "==", 2) or !strncmp(*_line, "!=", 2) or 
		!strncmp(*_line, ">=", 2) or !strncmp(*_line, "<=", 2) or
		!strncmp(*_line, ">", 1) or !strncmp(*_line, "<", 1))
	{
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getAddSub(_line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getAddSub(char** _line)
{
	skip_spaces(*_line);
	ASN* left_branch = getMulDiv(_line);
	
	skip_spaces(*_line);
	
	while (**_line == '+' or **_line == '-')
	{
		if (not left_branch)
		{
			left_branch = ast -> createNode(ASN::INT);
			left_branch -> ivalue = 0;
		}
		
		ASN* operand = ast -> createNode(_line);;
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getMulDiv(_line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getMulDiv(char** _line)
{
	skip_spaces(*_line);
	ASN* left_branch = getPow(_line);
	
	skip_spaces(*_line);
	while (**_line == '*' or **_line == '/' or **_line == '%')
	{
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getPow(_line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}


ASN* Source::getPow(char** _line)
{
	skip_spaces(*_line);
	ASN* left_branch = getNumVarFunc(_line);
	
	skip_spaces(*_line);
	if (**_line == '^')
	{
		ASN* operand = ast -> createNode(_line);
		operand -> leftConnect(left_branch);
		
		skip_spaces(*_line);
		ASN* right_branch = getNumVarFunc(_line);
		
		operand -> rightConnect(right_branch);
		left_branch = operand;
	}
	
	return left_branch;
}

	
ASN* Source::getNumVarFunc(char** _line)	// get numbers, variables and functions
{
	skip_spaces(*_line);
	
	if (**_line == '(')
	{
		(*_line)++;
		ASN* block = getLogic(_line);
		if (**_line != ')')
			std::cout << "Error!\n";
		(*_line)++;
		return block;
	}
		
	skip_spaces(*_line);
	
	if (**_line == '-')
		return 0;
	
	ASN* parsed = ast -> createNode(_line);
	
	skip_spaces(*_line);
	if (**_line == '(')
	{
		(*_line)++;
		ASN* itemize = getItemize(_line);
		if (itemize)
			parsed -> rightConnect(itemize);
		if (**_line != ')')
			std::cout << "Error!\n";
		(*_line)++;
	}
	
	return parsed;
}


ASN* Source::getItemize(char** _line)
{
	skip_spaces(*_line);
	if (**_line == ')')
		return 0;

	ASN* item = ast -> createNode(ASN::ITEM);
	item -> rightConnect(getLogic(_line));
	
	skip_spaces(*_line);
	if (**_line == ',')
	{
		(*_line)++;
		item -> leftConnect(getItemize(_line));
	}
	
	return item;
}


void Source::rebuildTree()
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


void Source::prepareAST()
{
	ASN* def = this -> ast -> head -> left;
	
	while (def)
	{
		ASN* function = def -> right;
		
		ASN* parameter = function -> right;		
		
		int varcounter = -1;
		while (parameter)
		{			
			varcounter++;
			setVariables(function, parameter -> right -> svalue, ASN::PARAMETER, varcounter);
			parameter = parameter -> left;
		}
		
		
		varcounter = -1;
		while (true)
		{
			char* unnumerated_variable = getUnnumeratedVariable(function);
			if (not unnumerated_variable)
				break;
				
			varcounter++;
			
			setVariables(function, unnumerated_variable, ASN::LOCAL, varcounter);
		}
		function -> ivalue = varcounter + 1;
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



char* Source::getUnnumeratedVariable(ASN* node)
{
	char* unnumerated_variable = 0;
	
	if (node -> type == ASN::VARIABLE and node -> ivalue == 0 and node -> vartype == 0)
		unnumerated_variable = node -> svalue;
		
	if (node -> right and not unnumerated_variable)
		unnumerated_variable = getUnnumeratedVariable(node -> right);
		
	if (node -> left and not unnumerated_variable)
		unnumerated_variable = getUnnumeratedVariable(node -> left);	
		
	return unnumerated_variable;
}



void Source::setVariables(ASN* node, const char* varname, int vartype, int varnumber)
{
	if (varname == 0)
		return;
	
	if (node -> type == ASN::VARIABLE and node -> svalue)
	{
		if (!strcmp(node -> svalue, varname))
		{
			node -> vartype = vartype;
			node -> ivalue = varnumber;
		}
	}
	
	if (node -> right)
		setVariables(node -> right, varname, vartype, varnumber);
	
	if (node -> left)
		setVariables(node -> left, varname, vartype, varnumber);
	
	return;
}


void Source::dumpAST()
{
	ast -> dumper(this -> name, AST::DELETE_TXT);
	printf("\x1b[1m%s:\x1b[0m Programm tree dump saved as \x1b[1;32m<%s.png>\x1b[0m\n", this -> name, this -> name);
}



