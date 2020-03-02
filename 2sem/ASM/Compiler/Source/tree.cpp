#pragma once

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
		
	  private:
		Tree* container;
	};
	
	Tree();
	~Tree();
	
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
