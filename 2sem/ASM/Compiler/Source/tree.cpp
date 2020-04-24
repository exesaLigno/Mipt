#pragma once
#include <cstdio>
//#include <unistd.h>
#include <cstdlib>
#include <cstring>

static const int SHOW = 1;
static const int DELETE_PNG = 10;
static const int DELETE_TXT = 100;
static const int DETAILED = 1000;

template <class D>
class Tree
{	
  public:

	class Node
	{		
		friend class Tree;
	  public:
		D data;
		Node* parent;
		Node* left;
		Node* right;
	
		Node(D data);
		~Node();
		
		void leftConnect(Node* left);
		void rightConnect(Node* right);
		
		Tree<D>::Node* leftPush(D data);
		Tree<D>::Node* rightPush(D data);
		
		int dumper(FILE* file, int mode);
		
	  private:
		Tree* container;
	};
	
	Tree();
	~Tree();
	
	int dumper(const char* filename, int mode);
	Node* createNode(D data);
	
	Node* head;
	
  private:
	unsigned long long int nodes_count;
};








template <class D>
Tree<D>::Node::Node (D data)
{
	this -> container = 0;
	this -> data = data;
	this -> parent = 0;
	this -> left = 0;
	this -> right = 0;
}

template <class D>
Tree<D>::Node::~Node()
{}


template <class D>
void Tree<D>::Node::leftConnect(Node* left)
{
	this -> left = left;
	left -> parent = this;
	this -> container -> nodes_count += 1;
}

template <class D>
void Tree<D>::Node::rightConnect(Node* right)
{
	this -> right = right;
	right -> parent = this;
	this -> container -> nodes_count += 1;
}


template <class D>
typename Tree<D>::Node* Tree<D>::Node::leftPush(D data)
{
	bool head_exists = false;
	if (this -> container -> head)
		head_exists = true;
		
	Node* new_node = this -> container -> createNode(data);
	
	if (head_exists)
		this -> leftConnect(new_node);
		
	return new_node;
}

template <class D>
typename Tree<D>::Node* Tree<D>::Node::rightPush(D data)
{
	bool head_exists = false;
	if (this -> container -> head)
		head_exists = true;
		
	Node* new_node = this -> container -> createNode(data);
	
	if (head_exists)
		this -> rightConnect(new_node);
		
	return new_node;
}



template <class D>
int Tree<D>::dumper(const char* filename, int mode)
{
	char* cmd = new char[2 * strlen(filename) + 1 + 8 + 16];
	strcpy(cmd, filename);
	strcat(cmd, ".txt");
    FILE* file = fopen (cmd, "w");
    fprintf(file, "digraph G{\n");
    fprintf(file, "root [shape = box, style = filled, fillcolor = orange, color = black, label = \" Root = %p\"]\n", this -> head);
    fprintf(file, "count [shape = box, style = filled, fillcolor = orange, color = black, label = \" Count of nodes = %lld\"]\n", this -> nodes_count);
    Tree<D>::Node* element = this -> head;
    
    fprintf(file, "%d [shape = record, style = filled, fillcolor = lightblue, color = black, label = \" {", element);
    
    if ((mode / 1000) % 10 == 1)
    	fprintf(file, "<adr> Address: %p | Data: ", element);
    
    fprintf(file, "%s", element -> data);
    
    if ((mode / 1000) % 10 == 1)
    	fprintf(file, " |{<left> %p | <right> %p}", element -> left, element -> right);
    	
    fprintf(file, "}\"]\n");
    
    fprintf(file, "root -> %d\n [color = black]", element);
    element -> dumper(file, mode);
    fprintf(file, "}");
    fclose(file);
    
    strcpy(cmd, "dot -Tpng ");
    strcat(cmd, filename);
    strcat(cmd, ".txt -o ");
    strcat(cmd, filename);
    strcat(cmd, ".png");
    system(cmd);
    
    if ((mode / 100) % 10 == 1)
    {
    	strcpy(cmd, "rm ");
    	strcat(cmd, filename);
    	strcat(cmd, ".txt");
    	system(cmd);
    }
    	
    if (mode % 10 == 1)
   	{
    	strcpy(cmd, "eog ");
    	strcat(cmd, filename);
    	strcat(cmd, ".png");
    	system(cmd);
    }
    	
    if ((mode / 10) % 10 == 1)
    {
    	strcpy(cmd, "rm ");
    	strcat(cmd, filename);
    	strcat(cmd, ".png");
    	system(cmd);
    }
    	
    delete[] cmd;
    	
    return 0;
}


template <class D>
int Tree<D>::Node::dumper(FILE* file, int mode)
{
    if (this -> left)
    {
        fprintf (file, "%d [shape = record, style = filled, fillcolor = lightblue, color = black, label = \" {", this -> left);
        
        if ((mode / 1000) % 10 == 1)
        	fprintf(file, "<adr> Address: %p | Data: ", this -> left);
        
        fprintf(file, "%s", this -> left -> data);
        
        if ((mode / 1000) % 10 == 1)
        	fprintf(file, " | <prev> Prev: %p |{<left> %p | <right> %p}", this, this -> left -> left, this -> left -> right);
        
        fprintf(file, "}\"]\n");
        
        fprintf (file, "%d:<left> -> %d [color = black]\n", this, this -> left);
        
        if ((mode / 1000) % 10 == 1)
        	fprintf (file, "%d:<prev> -> %d [color = gray]\n", this -> left, this);
        	
        this -> left -> dumper(file, mode);
    }

    if (this -> right)
    {
		fprintf (file, "%d [shape = record, style = filled, fillcolor = lightblue, color = black, label = \" {", this -> right);
        
        if ((mode / 1000) % 10 == 1)
        	fprintf(file, "<adr> Address: %p | Data: ", this -> right);
        
        fprintf(file, "%s", this -> right -> data);
        
        if ((mode / 1000) % 10 == 1)
        	fprintf(file, " | <prev> Prev: %p |{<left> %p | <right> %p}", this, this -> right -> left, this -> right -> right);
        
        fprintf(file, "}\"]\n");
		
        fprintf (file, "%d:<right> -> %d [color = black]\n", this, this -> right);
        
        if ((mode / 1000) % 10 == 1)
        	fprintf (file, "%d:<prev> -> %d [color = gray]\n", this -> right, this);
        	
		this -> right -> dumper(file, mode);
    }

    return 0;
}





template <class D>
Tree<D>::Tree()
{
	this -> head = 0;
	this -> nodes_count = 0;
}

template <class D>
Tree<D>::~Tree()
{}

template <class D>
typename Tree<D>::Node* Tree<D>::createNode(D data)
{
	Node* new_node = new Node(data);
	new_node -> container = this;
	if (not this -> head)
		this -> head = new_node;
	
	return new_node;
}
