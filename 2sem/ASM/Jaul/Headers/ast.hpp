#include <fstream>
#include <iostream>
#include <cstring>


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
	  	char* svalue = 0;
	  	bool LValue = 0;
	  	int vartype = 0;
	  	
	  	enum TYPES
	  	{
	  		UNKNOWN,
	  		ARITHM_OPERATOR, CMP_OPERATOR, CTRL_OPERATOR,		// OPERATORS
	  		VARIABLE, FUNCCALL, INT, FLOAT, STRING, CHAR,		// OPERANDS
	  		LINE, FUNC, ENTRY, ITEM, DEF,						// SPECIAL SYMBOLS
	  		_START,
	  		COUNT_OF_TYPES
	  	};
	  	
	  	enum OPERATORS
	  	{
	  		NOP,
	  		PLUS, MINUS, MULTIPLY, DIVIDE, POWER,				// ariphmetics operators
	  		MODULO, INT_DIVISION,
	  		
	  		EQUAL, NOT_EQUAL, MORE, LESS, MORE_EQ, LESS_EQ,		// comparison operators
	  		AND, OR,
	  		
	  		IF, ELSE, WHILE, FOR, RETURN, ASSIGNMENT,			// control operators
	  		PLUS_ASSIGNMENT, MINUS_ASSIGNMENT,
	  		MULTIPLY_ASSIGNMENT, DIVIDE_ASSIGNMENT,
	  		POWER_ASSIGNMENT,
	  		
	  		COUNT_OF_OPERATORS
	  	};
		
		Node* parent;
		Node* left;
		Node* right;
	
		Node(char** text);
		Node(const Node& that) = delete;
		~Node();
		
		void leftConnect(Node* left);
		void leftConnect(AbstractSyntaxTree* left);
		
		void rightConnect(Node* right);
		void rightConnect(AbstractSyntaxTree* right);
		
		void fixContainer();
		
		int dumper(std::ofstream& file, int mode);
		
		const char* colorize();
		friend std::ostream& operator<< (std::ostream &out, const Node &node);
		
	  private:
		AbstractSyntaxTree* container;
	};
	
	AbstractSyntaxTree();
	~AbstractSyntaxTree();
	
	int dumper(const char* filename, int mode);

	Node* createNode(char** text);
	
	Node* head;
	
  private:
	unsigned long long int nodes_count;	
};


bool isLetter(char symbol);
bool isNumber(char symbol);
bool isOperand(char symbol);
bool isEnd(char symbol);

bool isInteger(char* string);
bool isFloat(char* string);
bool isChar(char* string);
bool isString(char* string);
