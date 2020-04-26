#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
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
		
		int dumper(std::ofstream& file, int mode);
		
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
	if (this -> container)
		this -> container -> nodes_count += 1;
}

template <class D>
void Tree<D>::Node::rightConnect(Node* right)
{
	this -> right = right;
	right -> parent = this;
	if (this -> container)
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
	char* cmd = new char[2 * strlen(filename) + 1 + 8 + 29];
	strcpy(cmd, filename);
	strcat(cmd, ".txt");
    std::ofstream file;
    file.open(cmd);
    file << "digraph G{\n";
    file << "root [shape = box, style = filled, fillcolor = orange, color = black, label = \" Root = " << this -> head << "\"]\n";
    file << "count [shape = box, style = filled, fillcolor = orange, color = black, label = \" Count of nodes = " << this -> nodes_count << "\"]\n";
    Tree<D>::Node* element = this -> head;
    
    file << (long int) element << " [shape = record, style = filled, fillcolor = lightblue, color = black, label = \" {";	// element interprying as int
    
    if ((mode / 1000) % 10 == 1)
    	file << "<adr> Address: " << element << " | Data: ";
    
    file << element -> data;
    
    if ((mode / 1000) % 10 == 1)
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
int Tree<D>::Node::dumper(std::ofstream& file, int mode)
{
	
    if (this -> left)
    {
        file << (long int) this -> left << " [shape = record, style = filled, fillcolor = lightblue, color = black, label = \" {";
        
        if ((mode / 1000) % 10 == 1)
        	file << "<adr> Address: " << this -> left << " | Data: ";
        
        file << this -> left -> data;
        
        if ((mode / 1000) % 10 == 1)
        	file << " | <prev> Prev: " << this << " |{<left> " << this -> left -> left << " | <right> " << this -> left -> right << "}";
        
        file << "}\"]\n";
        
        if ((mode / 1000) % 10 == 1)
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
		file << (long int) this -> right << " [shape = record, style = filled, fillcolor = lightblue, color = black, label = \" {";
        
        if ((mode / 1000) % 10 == 1)
        	file << "<adr> Address: " << this -> right << " | Data: ";
        
        file << this -> right -> data;
        
        if ((mode / 1000) % 10 == 1)
        	file << " | <prev> Prev: " << this << " |{<left> " << this -> right -> left << " | <right> " << this -> right -> right << "}";
        
        file << "}\"]\n";
        
        if ((mode / 1000) % 10 == 1)
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
