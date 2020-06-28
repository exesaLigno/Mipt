#include "../Headers/ast.hpp"


AbstractSyntaxTree::Node::Node (char** text)
{
	
}


AbstractSyntaxTree::Node::~Node()
{
	if (this -> left)
		delete this -> left;

	if (this -> right)
		delete this -> right;
		
	if (this -> svalue)
		delete[] this -> svalue;
}


void AbstractSyntaxTree::Node::leftConnect(Node* left)
{
	if (left)
	{
		this -> left = left;
		left -> parent = this;
		if (this -> container)
		{
			this -> container -> nodes_count += 1;
			left -> container = this -> container;
			left -> fixContainer();
		}
	}
}


void AbstractSyntaxTree::Node::rightConnect(Node* right)
{
	if (right)
	{
		this -> right = right;
		right -> parent = this;
		if (this -> container)
		{
			this -> container -> nodes_count += 1;
			right -> container = this -> container;
			right -> fixContainer();
		}
	}
}



void AbstractSyntaxTree::Node::fixContainer()
{
	if (this -> right)
	{
		this -> container -> nodes_count += 1;
		this -> right -> container = this -> container;
		this -> right -> fixContainer();
	}
	
	if (this -> left)
	{
		this -> container -> nodes_count += 1;
		this -> left -> container = this -> container;
		this -> left -> fixContainer();
	}
}



const char* AbstractSyntaxTree::Node::colorize()
{
	if (this -> type == ENTRY)
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
    
//    if (this -> data_is_pointer)
//		file << *(element -> data);
//	else
//		file << element -> data;
    
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
        
//        if (this -> container -> data_is_pointer)
//			file << *(this -> left -> data);
//		else
//			file << this -> left -> data;
        
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
        
//        if (this -> container -> data_is_pointer)
//			file << *(this -> right -> data);
//		else
//			file << this -> right -> data;
        
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
