#include <fstream>
#include <cstdlib>
#include <locale>
#include <assert.h>
#include "TXLib.h"



struct node
{
    char*   data  = 0;
    node*   prev  = 0;
    node*   left  = 0;
    node*   right = 0;
};


struct tree_t
{
    node*   root = 0;
    int     count_of_nodes = 0;
};


template <typename elem_t>
node*   tree_lpush      (tree_t* tree, node* prev, elem_t element);

template <typename elem_t>
node*   tree_rpush      (tree_t* tree, node* prev, elem_t element);

int     branch_destroy  (tree_t* tree, node* element);
int     tree_destruct   (tree_t* tree);
int     tree_destruct   (node* element);



namespace service
{
    int     dot_dump        (tree_t* tree);
    int     dumper          (node* element, FILE* file);
}



template <typename elem_t>
node* tree_rpush (tree_t* tree, node* prev, elem_t element)
{
    char* data = (char*) calloc (50, sizeof (char));
    int counter = 0;
    while (element [counter] != '\0')
        data [counter] = element [counter++];
    data [counter] = '\0';

    node* new_node = (node*) calloc (1, (sizeof (node)));
    new_node -> data = data;

    new_node -> prev = prev;
    new_node -> left = 0;
    new_node -> right = 0;

    if (prev != 0)
        prev -> right = new_node;
    else
        tree -> root = new_node;
    return new_node;
}


template <typename elem_t>
node* tree_lpush (tree_t* tree, node* prev, elem_t element)
{
    char* data = (char*) calloc (50, sizeof (char));
    int counter = 0;
    while (element [counter] != '\0')
        data [counter] = element [counter++];
    data [counter] = '\0';

    node* new_node = (node*) calloc (1, (sizeof (node)));
    new_node -> data = data;

    new_node -> prev = prev;
    new_node -> left = 0;
    new_node -> right = 0;

    if (prev != 0)
        prev -> left = new_node;
    else
        tree -> root = new_node;
    return new_node;
}


int branch_destroy (tree_t* tree, node* element)
{
    if (element -> left)
        branch_destroy (tree, element -> left);
    if (element -> right)
        branch_destroy (tree, element -> right);

    element -> left = 0;
    element -> right = 0;
    element -> prev = 0;
    //element -> data = 0;
    return 0;
}


int tree_destruct (tree_t* tree)
{
    node* active_element = tree -> root;

    return 0;
}


int tree_destruct (node* element)
{
    if (!element -> left)
        tree_destruct (element -> left);

    if (!element -> right)
        tree_destruct (element -> right);

    free (element -> data);
    free (element);

    return 0;
}


int service::dot_dump (tree_t* tree)
{
    FILE* file = fopen ("graph_dump.txt", "w");
    fprintf (file, "digraph G{\n");
    node* element = tree -> root;
    fprintf (file, "%d [shape = box, style = rounded, color = blue, label = \"%s\"]\n", element, element -> data);
    dumper (element, file);
    fprintf (file, "}");
    fclose (file);
    system ("dot -Gcharset=utf-8 -Tpng graph_dump.txt -o dump.png");
    system ("del graph_dump.txt");
    system ("dump.png");
    return 0;
}


int service::dumper (node* element, FILE* file)
{
    if (element -> left)
    {
        fprintf (file, "%d [shape = box, style = rounded, color = blue, label = \"%s\"]\n", element -> left, element -> left -> data);
        fprintf (file, "%d -> %d [color = blue]\n", element, element -> left);
        dumper (element -> left, file);
    }

    if (element -> right)
    {
        fprintf (file, "%d [shape = box, style = rounded, color = blue, label = \"%s\"]\n", element -> right, element -> right -> data);
        fprintf (file, "%d -> %d [color = blue]\n", element, element -> right);
        dumper (element -> right, file);
    }

    return 0;
}

//end
