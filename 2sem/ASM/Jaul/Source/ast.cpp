#include "../Headers/ast.hpp"


AbstractSyntaxTree::Node::Node(char** text)
{
	this -> type = UNKNOWN;
	this -> ivalue = 0;
	this -> fvalue = 0;
	this -> cvalue = 0;
	this -> svalue = 0;
	this -> vartype = 0;
	this -> LValue = false;
	
	int first_sym = 0;
	if (isLetter(**text))
		first_sym = LETTER;
		
	if (isNumber(**text))
		first_sym = NUMBER;
		
	if (isOperand(**text))
		first_sym = SPECSYMBOL;
		
	int token_len = 0;
	char* token_start = *text;
	bool in_token = true;
	
	while (in_token)
	{
		token_len++;
		(*text)++;
		
		if (((first_sym == LETTER or first_sym == NUMBER) and (isOperand(**text) or isEnd(**text))) or	\
		   (first_sym == SPECSYMBOL and (not isOperand(**text))))
			in_token = false;
	}
	
	char* token = new char[token_len + 1]{0};
	strncpy(token, token_start, token_len);
	
	if(0)
		std::cout << "\x1b[1;31mbullshit\n\x1b[0m";

	#define TOKEN(string, token_type, token_number, dump, nasm_code, bin_code)		\
			else if (!strcmp(token, string))										\
			{																		\
				this -> type = token_type;											\
				this -> ivalue = token_number;										\
				delete[] token;														\
			}																		\
			
	#include "../Syntax/jaul_syntax.hpp"
	
	#undef TOKEN
	
	else if (isInteger(token))
	{
		this -> type = INT;
		this -> ivalue = atoi(token);
		delete[] token;
	}
	
	else if (isFloat(token))
	{
		this -> type = FLOAT;
		this -> fvalue = atof(token);
		delete[] token;
	}
	
	else if (isChar(token))
	{
		this -> type = CHAR;
		this -> cvalue = token[1];
		delete[] token;
	}
	
	else if (isString(token))
	{
		this -> type = STRING;
		delete[] token;
	}

	else
	{
		char* test_next = *text;
		
		while (*test_next == ' ')
			(test_next)++;
			
		if (*test_next == '(')
			this -> type = FUNCCALL;
			
		else
			this -> type = VARIABLE;
			
		if (*test_next == '=' and *(test_next + 1) != '=')	
			this -> LValue = true;
			
		this -> svalue = token;
	}
}


AbstractSyntaxTree::Node::Node(int type)
{
	this -> type = type;
	this -> ivalue = 0;
	this -> fvalue = 0;
	this -> cvalue = 0;
	this -> svalue = 0;
	this -> vartype = 0;
	this -> LValue = false;
	if (type == _START)
	{
		this -> type = FUNC;
		this -> svalue = new char[10];
		strcpy(this -> svalue, "_start");
	}
}


AbstractSyntaxTree::Node::Node(int type, char** text)
{
	this -> type = type;
	this -> ivalue = 0;
	this -> fvalue = 0;
	this -> cvalue = 0;
	this -> svalue = 0;
	this -> vartype = 0;
	this -> LValue = false;
	
	int first_sym = 0;
	if (isLetter(**text))
		first_sym = LETTER;
		
	if (isNumber(**text))
		first_sym = NUMBER;
		
	if (isOperand(**text))
		first_sym = SPECSYMBOL;
		
	int token_len = 0;
	char* token_start = *text;
	bool in_token = true;
	
	while (in_token)
	{
		token_len++;
		(*text)++;
		
		if (((first_sym == LETTER or first_sym == NUMBER) and (isOperand(**text) or isEnd(**text))) or	\
		   (first_sym == SPECSYMBOL and (not isOperand(**text))))
			in_token = false;
	}
	
	char* token = new char[token_len + 1]{0};
	strncpy(token, token_start, token_len);
	
	this -> svalue = token;
}


AbstractSyntaxTree::Node::~Node()
{
	if (this -> left)
		delete this -> left;

	if (this -> right)
		delete this -> right;
		
	if (this -> svalue)
		delete[] this -> svalue;
		
	if (this -> container)
		(this -> container -> nodes_count)--;
}


void AbstractSyntaxTree::Node::leftConnect(Node* left)
{
	this -> left = left;
	if (left)
	{
		left -> parent = this;
		if (this -> container)
		{
			if (this -> left -> container != this -> container)
			{
				this -> container -> nodes_count += 1;
				this -> left -> container = this -> container;
			}
			left -> fixContainer();
		}
	}
}


void AbstractSyntaxTree::Node::rightConnect(Node* right)
{
	this -> right = right;
	if (right)
	{
		right -> parent = this;
		if (this -> container)
		{
			if (this -> right -> container != this -> container)
			{
				this -> container -> nodes_count += 1;
				this -> right -> container = this -> container;
			}
			right -> fixContainer();
		}
	}
}



void AbstractSyntaxTree::Node::fixContainer()
{
	if (this -> right)
	{
		if (this -> right -> container != this -> container)
		{
			this -> container -> nodes_count += 1;
			this -> right -> container = this -> container;
		}
		this -> right -> fixContainer();
	}
	
	if (this -> left)
	{
		if (this -> left -> container != this -> container)
		{
			this -> container -> nodes_count += 1;
			this -> left -> container = this -> container;
		}
		this -> left -> fixContainer();
	}
}



const char* AbstractSyntaxTree::Node::colorize()
{
	if (this -> type == ENTRY or this -> type == INCLUDE)
		return "green";
		
	else if (this -> type == LINE or this -> type == ITEM or this -> type == DEF)
		return "lightgray";
		
	else if (this -> type == FUNC)
		return "green";
	
	else if (this -> type == ARITHM_OPERATOR)
		return "pink";
		
	else if (this -> type == CMP_OPERATOR)
		return "\"#f8aeff\"";
		
	else if (this -> type == CTRL_OPERATOR)
		return "orange";
		
	else if (this -> type == INT)
		return "\"#D083FF\"";
		
	else if (this -> type == CHAR)
		return "\"#B3FFEF\"";
		
	else if (this -> type == FLOAT)
		return "\"#FF89A9\"";
		
	else if (this -> type == STRING)
		return "\"#9FFF88\"";
		
	else if (this -> type == VARIABLE)
		return "\"#64ADFF\"";
		
	else if (this -> type == FUNCCALL)
		return "\"#0B8AFF\"";
		
	else
		return "lightblue";
}


void AbstractSyntaxTree::Node::write(std::ofstream& out)
{
	if (this -> type == UNKNOWN)
		out << "unknown | " << this -> svalue;
		
	else if (this -> type == ENTRY)
		out << "entry";
		
	else if (this -> type == LINE)
	{
		out << "line " << this -> ivalue;
		out << " | " << this -> svalue;
	}
	
	else if (this -> type == INCLUDE)
	{
		out << "include";
		out << " | " << this -> svalue;
	}
		
	else if (this -> type == FUNC)
		out << "function | " << this -> svalue << " | { Mem | " << this -> ivalue << " } ";
		
	else if (this -> type == ITEM)
		out << "item";

	#define TOKEN(string, token_type, token_number, dump, nasm_code, bin_code)					\
			else if (this -> type == token_type and this -> ivalue == token_number)				\
				out << "OPERATOR | " << dump;													\
			
	#include "../Syntax/jaul_syntax.hpp"
	
	#undef TOKEN

	else if (this -> type == INT)
		out << "int | " << this -> ivalue;
		
	else if (this -> type == FLOAT)
		out << "float | " << this -> fvalue;
		
	else if (this -> type == CHAR)
		out << "char | " << this -> cvalue;
		
	else if (this -> type == STRING)
		out << "string | " << this -> svalue;
		
	else if (this -> type == VARIABLE)
	{
		if (this -> LValue)
			out << "L-";
		out << "variable | " << this -> svalue;
			
		if (this -> vartype == LOCAL)
			out << " | { Local | " << this -> ivalue << " }";
			
		else if (this -> vartype == PARAMETER)
			out << " | { Parameter  | " << this -> ivalue << " }";
	}
		
	else if (this -> type == FUNCCALL)
		out << "funccall | " << this -> svalue;
		
	else if (this -> type == DEF)
	{
		out << "def";
		out << " | line " << this -> ivalue;
		if (this -> svalue)
			out << " | " << this -> svalue;
	}
}


int AbstractSyntaxTree::dumper(const char* filename, int mode)
{
	char* cmd = new char[2 * strlen(filename) + 1 + 8 + 29];
	strcpy(cmd, filename);
	strcat(cmd, ".txt");
    std::ofstream file;
    file.open(cmd);
    file << "digraph G{\n";
    file << "root [shape = box, style = filled, fillcolor = orange, color = black, label = \" Root = " << this -> head << "\"]\n";
    file << "count [shape = box, style = filled, fillcolor = orange, color = black, label = \" Count of nodes = " << this -> nodes_count << "\"]\n";
    AbstractSyntaxTree::Node* element = this -> head;
    
    file << (long int) element << " [shape = record, style = filled, fillcolor = " << element -> colorize() << ", color = black, label = \" {";
    
    if ((mode / DETAILED) % 10 == 1)
    	file << "<adr> Address: " << element << " | Data: ";
    
	element -> write(file);
    
    if ((mode / DETAILED) % 10 == 1)
    	file << " |{<left> " << element -> left << " | <right> " << element -> right << "}";
    	
    file << "}\"]\n";
    
    file << "root -> " << (long int) element << "\n [color = black]";	// element as int
	
    element -> dumper(file, mode);
    file << "}";
    file.close();
    
    strcpy(cmd, "dot -Tpng ");
    strcat(cmd, filename);
    strcat(cmd, ".txt -o ");
    strcat(cmd, filename);
    strcat(cmd, ".png");
    int result = system(cmd);
    
    if ((mode / DELETE_TXT) % 10 == 1)
    {
    	strcpy(cmd, "rm ");
    	strcat(cmd, filename);
    	strcat(cmd, ".txt");
    	result = system(cmd);
    }
    	
    if ((mode / SHOW) % 10 == 1)
   	{
    	strcpy(cmd, "eog ");
    	strcat(cmd, filename);
    	strcat(cmd, ".png");
    	result = system(cmd);
    }
    	
    if ((mode / DELETE_PNG) % 10 == 1)
    {
    	strcpy(cmd, "rm ");
    	strcat(cmd, filename);
    	strcat(cmd, ".png");
    	result = system(cmd);
    }
    	
    delete[] cmd;
    	
    return 0;
}



int AbstractSyntaxTree::Node::dumper(std::ofstream& file, int mode)
{
	
    if (this -> left)
    {
        file << (long int) this -> left << " [shape = record, style = filled, fillcolor = " << this -> left -> colorize() << ", color = black, label = \" {";
        
        if ((mode / DETAILED) % 10 == 1)
        	file << "<adr> Address: " << this -> left << " | Data: ";
        
		this -> left -> write(file);
        
        if ((mode / DETAILED) % 10 == 1)
        	file << " | <prev> Prev: " << this << " |{<left> " << this -> left -> left << " | <right> " << this -> left -> right << "}";
        
        file << "}\"]\n";
        
        if ((mode / DETAILED) % 10 == 1)
        {
        	file << (long int) this << ":<left> -> " << (long int) this -> left << " [color = black]\n";
        	file << (long int) this -> left << ":<prev> -> " << (long int) this << " [color = gray]\n";
        }
        	
        else
        	file << (long int) this << " -> " << (long int) this -> left << " [color = black]\n";
        	
        this -> left -> dumper(file, mode);
    }

    if (this -> right)
    {
		file << (long int) this -> right << " [shape = record, style = filled, fillcolor = " << this -> right -> colorize() << ", color = black, label = \" {";
        
        if ((mode / DETAILED) % 10 == 1)
        	file << "<adr> Address: " << this -> right << " | Data: ";
        
		this -> right -> write(file);
        
        if ((mode / DETAILED) % 10 == 1)
        	file << " | <prev> Prev: " << this << " |{<left> " << this -> right -> left << " | <right> " << this -> right -> right << "}";
        
        file << "}\"]\n";
        
        if ((mode / DETAILED) % 10 == 1)
        {
        	file << (long int) this << ":<right> -> " << (long int) this -> right << " [color = black]\n";
        	file << (long int) this -> right << ":<prev> -> " << (long int) this << " [color = gray]\n";
        }
        
        else
        	file << (long int) this << " -> " << (long int) this -> right << " [color = black]\n";
        	
        this -> right -> dumper(file, mode);
    }

    return 0;
}






AbstractSyntaxTree::AbstractSyntaxTree()
{
	this -> head = 0;
	this -> nodes_count = 0;
}



AbstractSyntaxTree::~AbstractSyntaxTree()
{
	if (this -> head)
		delete this -> head;
}


typename AbstractSyntaxTree::Node* AbstractSyntaxTree::createNode(char** text)
{
	Node* new_node = new Node(text);
	this -> nodes_count++;
	new_node -> container = this;
	if (not this -> head)
		this -> head = new_node;
	
	return new_node;
}

typename AbstractSyntaxTree::Node* AbstractSyntaxTree::createNode(int type, char** text)
{
	Node* new_node = new Node(type, text);
	this -> nodes_count++;
	new_node -> container = this;
	if (not this -> head)
		this -> head = new_node;
	
	return new_node;
}

typename AbstractSyntaxTree::Node* AbstractSyntaxTree::createNode(int type)
{
	Node* new_node = new Node(type);
	this -> nodes_count++;
	new_node -> container = this;
	if (not this -> head)
		this -> head = new_node;
	
	return new_node;
}




bool isLetter(char symbol)
{
	return ((symbol >= 'A' and symbol <= 'Z') or (symbol >= 'a' and symbol <= 'z') or symbol == '.' or symbol == '_' or symbol == '\'' or symbol == '\"' or symbol == '-');
}

bool isNumber(char symbol)
{
	return ((symbol >= '0' and symbol <= '9') or symbol == '-');
}

bool isOperand(char symbol)
{
	return (symbol == '>' or symbol == '<' or symbol == '=' or symbol == '!' or \
			symbol == '+' or symbol == '-' or symbol == '*' or symbol == '/' or \
			symbol == '%' or symbol == '|' or symbol == '&' or symbol == '^');
}

bool isEnd(char symbol)
{
	return (symbol == ' ' or symbol == '\t' or symbol == '\n' or symbol == '\0' or 
			symbol == '(' or symbol == ')' or symbol == ',');
}



bool isInteger(char* string)
{
	bool result = true;
	
	while (*string != '\0')
	{
		if (not (isNumber(*string)))
			result = false;
		string++;
	}
	
	return result;
}

bool isFloat(char* string)
{
	bool result = true;
	
	while (*string != '\0')
	{
		if (not (isNumber(*string) or *string == '.'))
			result = false;
		string++;
	}
	
	return result;
}

bool isChar(char* string)
{
	bool result = true;
	
	if (*string != '\'')
		result = false;
	
	while (*string != '\0')
		string++;

	string--;
	
	if (*string != '\'')
		result = false;
	
	return result;
}


bool isString(char* string)
{
	bool result = true;
	
	if (*string != '\"')
		result = false;
	
	while (*string != '\0')
		string++;

	string--;
	
	if (*string != '\"')
		result = false;
	
	return result;
}
