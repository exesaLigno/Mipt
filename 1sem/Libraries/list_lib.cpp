#include <fstream>
#include <cstdlib>
#include <locale>
#include <assert.h>



namespace ERR
{
    const int NORMAL = 0;
}



struct node
{
    node*   next  = 0;
    node*   prev  = 0;
    char    data [0];
};


struct list_t
{
    node*   head = 0;
    int     count_of_nodes = 0;
};


template <typename elem_t>
node*   list_ainsert    (list_t* list, node* prev, elem_t element);

template <typename elem_t>
node*   list_binsert    (list_t* list, node* next, elem_t element);

int     list_remove     (list_t* list, node* element);


namespace slow
{
    int     list_index_realtoord    (list_t* list, int real_index);
    int     list_index_ordtoreal    (list_t* list, int ordinal_index);
    int     list_sort               (list_t* list);
    int     list_compare            (const void* elem1, const void* elem2);
    int     list_defrag             (list_t* list);
    int     list_expand             (list_t* list, int new_length);
    int     list_squeeze            (list_t* list, int new_length);
}


namespace service
{
    int     int_dot_dump    (list_t* list);
    int     str_dot_dump    (list_t* list);
    int     dumper          (node* element, FILE* file, int flag);
    int     dumper          (node* element, FILE* file, char* flag);

    int     verificate      (list_t* list);
    int     verificate      (node* element, int* counter);
}



int main ()
{
    setlocale (LC_CTYPE, "Russian");

    tree_t tree;

    char a [5] = "mas";
    char* b = (char*) calloc (5, sizeof (char));
    b = "cal";

    node* last = tree_rpush (&tree, 0, "1");
    node* last2 = tree_lpush (&tree, last, "21");
    tree_rpush (&tree, last, b);
    tree_lpush (&tree, last2, a);
    node* last3 = tree_rpush (&tree, last2, "dick");
    tree_lpush (&tree, last3, "3");
    node* last4 = tree_rpush (&tree, last3, "2");

    tree_construct (&tree, last4, 0, 7);

    //branch_destroy (&tree, last3);

    //printf ("%d\n", verificate (&tree));

    str_dot_dump (&tree);

    return 0;
}


template <typename elem_t>
node* tree_rpush (tree_t* tree, node* prev, elem_t element)
{
    node* new_node = (node*) calloc (1, (sizeof (node) + sizeof (elem_t)));
    *(elem_t*) new_node -> data = element;
    new_node -> prev = prev;
    new_node -> left = 0;
    new_node -> right = 0;
    (tree -> count_of_nodes)++;
    if (prev != 0)
        prev -> right = new_node;
    else
        tree -> root = new_node;
    return new_node;
}


template <typename elem_t>
node* tree_lpush (tree_t* tree, node* prev, elem_t element)
{
    node* new_node = (node*) calloc (1, (sizeof (node) + sizeof (elem_t)));
    *(elem_t*) new_node -> data = element;
    new_node -> prev = prev;
    new_node -> left = 0;
    new_node -> right = 0;
    (tree -> count_of_nodes)++;
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
    if (element -> prev)
    {
        if (element -> prev -> left == element)
            element -> prev -> left = 0;
        else if (element -> prev -> right == element)
            element -> prev -> right = 0;
        else
            return ERR::LINKING_ERROR;
    }
    element -> prev = 0;
    *element -> data = 0;

    (tree -> count_of_nodes)--;

    free (element);

    return 0;
}


int int_dot_dump (tree_t* tree)
{
    FILE* file = fopen ("graph_dump.txt", "w");
    fprintf (file, "digraph G{\n");
    fprintf (file, "root [shape = box, style = rounded, color = red, label = \" Root = %p\"]\n", tree -> root);
    fprintf (file, "count [shape = box, style = rounded, color = red, label = \" Count of nodes = %d\"]\n", tree -> count_of_nodes);
    node* element = tree -> root;
    fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<adr> Address: %p | Data: %d |{<left> %p | <right> %p}}\"]\n", element, element, *(int*) element -> data, element -> left, element -> right);
    fprintf (file, "root -> %d\n [color = blue]", element);
    dumper (element, file, 0);
    fprintf (file, "}");
    fclose (file);
    system ("dot -Tpng graph_dump.txt -o dump.png");
    system ("del graph_dump.txt");
    system ("dump.png");
    return 0;
}


int str_dot_dump (tree_t* tree)
{
    FILE* file = fopen ("graph_dump.txt", "w");
    fprintf (file, "digraph G{\n");
    fprintf (file, "root [shape = box, style = rounded, color = red, label = \" Root = %p\"]\n", tree -> root);
    fprintf (file, "count [shape = box, style = rounded, color = red, label = \" Count of nodes = %d\"]\n", tree -> count_of_nodes);
    node* element = tree -> root;
    fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<adr> Address: %p | Data: %s |{<left> %p | <right> %p}}\"]\n", element, element, *(char**) element -> data, element -> left, element -> right);
    fprintf (file, "root -> %d\n [color = blue]", element);
    dumper (element, file, "");
    fprintf (file, "}");
    fclose (file);
    system ("dot -Tpng graph_dump.txt -o dump.png");
    system ("del graph_dump.txt");
    system ("dump.png");
    return 0;
}


int dumper (node* element, FILE* file, int flag)
{
    if (element -> left)
    {
        fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<adr> Address: %p | Data: %d | <prev> Prev: %p |{<left> %p | <right> %p}}\"]\n", element -> left, element -> left, *(int*) element -> left -> data, element, element -> left -> left, element -> left -> right);
        fprintf (file, "%d:<left> -> %d [color = blue]\n", element, element -> left);
        fprintf (file, "%d:<prev> -> %d [color = gray]\n", element -> left, element);
        dumper (element -> left, file, 0);
    }

    if (element -> right)
    {
        fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<adr> Address: %p | Data: %d | <prev> Prev: %p |{<left> %p | <right> %p}}\"]\n", element -> right, element -> right, *(int*) element -> right -> data, element, element -> right -> left, element -> right -> right);
        fprintf (file, "%d:<right> -> %d [color = blue]\n", element, element -> right);
        fprintf (file, "%d:<prev> -> %d [color = gray]\n", element -> right, element);
        dumper (element -> right, file, 0);
    }

    return 0;
}


int dumper (node* element, FILE* file, char* flag)
{
    if (element -> left)
    {
        fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<adr> Address: %p | Data: %s | <prev> Prev: %p |{<left> %p | <right> %p}}\"]\n", element -> left, element -> left, *(char**) element -> left -> data, element, element -> left -> left, element -> left -> right);
        fprintf (file, "%d:<left> -> %d [color = blue]\n", element, element -> left);
        fprintf (file, "%d:<prev> -> %d [color = gray]\n", element -> left, element);
        dumper (element -> left, file, "");
    }

    if (element -> right)
    {
        fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<adr> Address: %p | Data: %s | <prev> Prev: %p |{<left> %p | <right> %p}}\"]\n", element -> right, element -> right, *(char**) element -> right -> data, element, element -> right -> left, element -> right -> right);
        fprintf (file, "%d:<right> -> %d [color = blue]\n", element, element -> right);
        fprintf (file, "%d:<prev> -> %d [color = gray]\n", element -> right, element);
        dumper (element -> right, file, "");
    }

    return 0;
}


int verificate (tree_t* tree)
{
    int correct_tree = 0;
    int counter = 0;

    int err = verificate (tree -> root, &counter);

    if (tree -> count_of_nodes != counter)
        return ERR::CYCLED_TREE;

    return err;
}


int verificate (node* element, int* counter)
{
    int err_left = 0;
    int err_right = 0;

    (*counter)++;

    if (element -> left)
        err_left = verificate (element -> left, counter);

    if (element -> right)
        err_right = verificate (element -> right, counter);

    int err = std::max (err_left, err_right);

    return err;
}

/*
int print (&tree)
{
    return 0;
}
*/
//end
