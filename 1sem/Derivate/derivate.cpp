#include <fstream>
#include "tree_lib.cpp"
#include <cstdlib>
#include <cstring>
#include <assert.h>
#include "defines.h"
#include "service.h"

#define meow




struct func_element
{
    int     type        = 0;
    int     op          = 0;
    double  number      = 0;
    char    constanta   = 0;
    char    variable    = 0;
};




int             read_function   (char** func_text, tree_t* func);
int             read_function   (char** func_text, tree_t* func, node* last_added, int* counter, int side);
int             new_read        (char func_text [], tree_t* func);
func_element    analize         (char buffer []);

tree_t          derivate        (tree_t* func);
node*           derivate_branch (tree_t* func_derivated, node* element);
node*           copy_branch     (tree_t* func_derivated, node* element);

node*           operator+       (node &left_element, node &right_element);
node*           operator-       (node &left_element, node &right_element);
node*           operator*       (node &left_element, node &right_element);
node*           operator/       (node &left_element, node &right_element);

namespace optimization
{
    int optimize    (tree_t* func);
    int o1          (tree_t* func);
    int o1          (tree_t* func, node* element);
    int o2          (tree_t* func);
    int o2          (tree_t* func, node* element);
}

int         create_tex_file     ();
int         complete_tex_file   ();

int             print           (tree_t* func);
int             print           (node* element);
int             fprintf         (FILE* file, tree_t* func);
int             fprintf         (FILE* file, node* element);

node*    GetG0   (char buffer []);
node*    GetE    ();
node*    GetT    ();
node*    GetP    ();
node*    GetN    ();

namespace service
{
    FILE* tex_file = fopen ("dump.txt", "w");

    const int HIDDEN = 0;
    const int SHOW = 1;
    int dump (tree_t* tree, char filename [], int mode);
    int dump (node* element, FILE* file);

    int tex_dump (tree_t* func);
}




namespace global
{
    const long func_size = 100;
}


namespace type
{
    const int op_t = 1;
    const int number = 2;
    const int constanta = 3;
    const int variable = 4;
}


namespace reading
{
    char* line = 0;
    int position = 0;
}


#define DEF_OP( op, num, name, code )     \
        o##name = num,            \

enum op
{
    #include "operators.h"
    op_count
};

#undef DEF_OP




int main ()
{
    create_tex_file ();

    //char* func_text = "+(*(x)(x))(+(2.6)(/(3)(x)))"; ///x^2+3/x+2.6
    char func_text_normal [global::func_size] = "x*(x+3)/x-5-8";

    ///Hard func  *(+(*(x)(x))(+(*(2.6)(x))(/(3)(x))))(*(3)(x))
    ///Easy func  +(*(x)(x))(+(2.6)(/(3)(x)))
    //char* func_text = (char*) calloc (global::func_size, sizeof (char));
    //scanf ("%s", func_text);
    tree_t func;

    //read_function (&func_text, &func);

    new_read (func_text_normal, &func);

    //service::dump (&func, "before.png", service::SHOW);

    tree_t func_derivated = derivate (&func);

    int_dot_dump (&func_derivated);

    //tree_fix_prev (&func_derivated);
    //service::dump (&func_derivated, "test.png", service::SHOW);

    //service::dump (&func_derivated, "after.png");

    //print (&func_derivated);
    //printf ("\n");

    optimization::optimize (&func_derivated);

    print (&func_derivated);

    service::dump (&func_derivated, "optimized_once.png", service::SHOW);

    complete_tex_file ();

    return 0;
}



int create_tex_file ()
{
    fprintf (service::tex_file, "\\documentclass[12pt]{article}\n");
    fprintf (service::tex_file, "\\usepackage[utf8]{inputenc}\n");
    fprintf (service::tex_file, "\\usepackage[russian]{babel}\n");
    fprintf (service::tex_file, "\\usepackage{cite}\n");
    fprintf (service::tex_file, "\\usepackage{amsfonts}\n");
    fprintf (service::tex_file, "\\usepackage{lineno}\n");
    fprintf (service::tex_file, "\\usepackage{hyperref}\n");
    fprintf (service::tex_file, "\\usepackage{subfig}\n");
    fprintf (service::tex_file, "\\usepackage{graphicx}\n");
    fprintf (service::tex_file, "\\usepackage{xcolor}\n");
    fprintf (service::tex_file, "\\usepackage{bm}\n");
    fprintf (service::tex_file, "\\usepackage{bbm}\n\n");

    fprintf (service::tex_file, "\\thispagestyle{empty}\n");
    fprintf (service::tex_file, "\\hoffset = -2.2cm\n");
    fprintf (service::tex_file, "\\voffset = -3.3cm\n");
    fprintf (service::tex_file, "\\textwidth = 18.1cm\n");
    fprintf (service::tex_file, "\\textheight = 25.7cm\n\n");

    fprintf (service::tex_file, "\\usepackage{color}\n\n");

    fprintf (service::tex_file, "\\begin{document}\n\n");
}


int complete_tex_file ()
{
    fprintf (service::tex_file, "\n\\end{document}\n");
    fclose (service::tex_file);
    return 0;
}


node* operator+ (node &left_element, node &right_element)
{
    func_element op;
    op.type = type::op_t;
    op.op = oplus;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator- (node &left_element, node &right_element)
{
    func_element op;
    op.type = type::op_t;
    op.op = ominus;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator* (node &left_element, node &right_element)
{
    func_element op;
    op.type = type::op_t;
    op.op = omul;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator/ (node &left_element, node &right_element)
{
    func_element op;
    op.type = type::op_t;
    op.op = odiv;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


int read_function (char** func_text, tree_t* func)
{
    int counter = 0;
    read_function (func_text, func, 0, &counter, 0);

    return 0;
}


int read_function (char** func_text, tree_t* func, node* last_added, int* counter, int side)
{
    const int LEFT = -1;
    const int RIGHT = 1;

    int buffer_length = 10;
    char buffer [buffer_length] = {};
    int local_counter = 0;

    if (*counter)
        while ((*func_text) [(*counter) - 1] != '(') (*counter)++;

    while (((*func_text) [*counter] != '(') && ((*func_text) [*counter] != ')'))
        buffer [local_counter++] = (*func_text) [(*counter)++];

    buffer [local_counter] = '\0';

    func_element element = analize (buffer);

    node* current = 0;

    if (side == LEFT) current = tree_lpush (func, last_added, element);
    else if (side == RIGHT) current = tree_rpush (func, last_added, element);
    else current = tree_lpush (func, last_added, element);

    if ((*func_text) [*counter] == ')')
    {
        while ((*func_text) [*counter] == ')') (*counter)++;
        return 0;
    }

    if ((*func_text) [*counter] == '(')
        read_function (func_text, func, current, counter, LEFT);

    if ((*func_text) [*counter] == '(')
        read_function (func_text, func, current, counter, RIGHT);

    return 0;
}


int new_read (char func_text [], tree_t* func)
{
    func -> root = GetG0 (func_text);
    tree_fix_prev (func);
    return 0;
}


node* GetG0 (char buffer [])
{
    reading::line = buffer;
	reading::position = 0;
	node* element = GetE ();
	//assert (reading::line [reading::position] == 0);
	return element;
}


node* GetE ()
{
    node* element = GetT ();
	while (reading::line [reading::position] == '+' || reading::line [reading::position] == '-')
	{
		int op = reading::line [reading::position];
		reading::position++;
		node* element2 = GetT ();
		if (op == '+')
            element = *element + *element2;
		else if (op == '-')
            element = *element - *element2;
	}
    return element;
}


node* GetT ()
{
    node* element = GetP ();
	while (reading::line [reading::position] == '*' || reading::line [reading::position] == '/')
	{
		int op = reading::line [reading::position];
		reading::position++;
		node* element2 = GetP ();
		if (op == '*')
            element = *element * *element2;
		else if (op == '/')
            element = *element / *element2;
	}
    return element;
}


node* GetP ()
{
    if (reading::line [reading::position] == '(')
	{
		reading::position++;
		node* element = GetE ();
		assert (reading::line [reading::position] == ')');
		reading::position++;
		return element;
	}
	else
		return GetN ();
}


node* GetN ()
{
    int buffer_size = 10;
    char buffer [buffer_size] = {};
    int buffer_counter = 0;

	while (('0' <= reading::line [reading::position] && reading::line [reading::position] <= '9') || ('a' <= reading::line [reading::position] && reading::line [reading::position] <= 'z'))
		buffer [buffer_counter++] = reading::line [reading::position++];

    func_element value = analize (buffer);

    return create_node (value);
}



func_element analize (char buffer [])
{
    func_element element;

    if (atof (buffer) || buffer [0] == '0')
    {
        element.type = type::number;
        element.number = atof (buffer);
    }

    else if (!strcmp (buffer, "x"))
    {
        element.type = type::variable;
        element.variable = buffer [0];
    }

    #define DEF_OP( operator, num, name, code )     \
            else if (!strcmp (buffer, operator))    \
            {                                       \
                element.type = type::op_t;          \
                element.op = o##name;               \
            }                                       \

    #include "operators.h"

    #undef DEF_OP

    else
    {
        element.type = type::constanta;
        element.constanta = buffer [0];
    }

    return element;
}


tree_t derivate (tree_t* func)
{
    fprintf (service::tex_file, "Найдем производную функции:\n\n");
    fprintf (service::tex_file, "\\begin{equation}\n");

    fprintf (service::tex_file, "    y(x) = ");
    fprintf (service::tex_file, func);

    fprintf (service::tex_file, "\n\\end{equation}\n\n\n");

    tree_t func_derivated;
    node* root = derivate_branch (&func_derivated, func -> root);

    func_derivated.root = root;

    tree_fix_prev (&func_derivated);

    fprintf (service::tex_file, "Таким образом, получим итоговое выражение:\n\n");

    fprintf (service::tex_file, "\\begin{equation}\n");

    fprintf (service::tex_file, "    y'(x) = ");
    fprintf (service::tex_file, &func_derivated);

    fprintf (service::tex_file, "\n\\end{equation}\n");

    return func_derivated;
}


node* derivate_branch (tree_t* func_derivated, node* element)
{
    func_element filler;
    node* new_node = create_node (filler);

    if (eltype == type::constanta)
    {
        newtype = type::number;
        newnumber = 0;
    }

    if (eltype == type::number)
    {
        newtype = type::number;
        newnumber = 0;
    }

    if (eltype == type::variable)
    {
        newtype = type::number;
        newnumber = 1;
    }

    if (eltype == type::op_t)
    {
        #define DEF_OP( operator, num, name, code )         \
                case num:                                   \
                {                                           \
                    code                                    \
                    break;                                  \
                }                                           \

        switch (elop)
        {
            #include "operators.h"
            default: printf ("unknown operator detected"); break;
        }

        #undef DEF_OP

        fprintf (service::tex_file, "\n\nНайдем производную от $");
        fprintf (service::tex_file, element);
        fprintf (service::tex_file, "$, получим $");
        fprintf (service::tex_file, new_node);
        fprintf (service::tex_file, "$\n\n");
    }

    return new_node;
}


node* copy_branch (tree_t* func_derivated, node* element)
{
    func_element filler;
    node* new_node = create_node (filler);

    *new_node = *element;
    *(func_element*) new_node -> data = *(func_element*) element -> data;

    if (element -> left)
        tree_lconnect (func_derivated, element, copy_branch (func_derivated, element -> left));

    if (element -> right)
        tree_rconnect (func_derivated, element, copy_branch (func_derivated, element -> right));

    return new_node;
}


int optimization::optimize (tree_t* func)
{
    fprintf (service::tex_file, "\n\nЭта функция слишком сложная для понимания, попробуем ее упростить.\n\n");

    int continue_condition = 1;
    int first = 1;
    while (continue_condition)
    {
        fprintf (service::tex_file, "Путем несложных преобразований из $");
        fprintf (service::tex_file, func);

        continue_condition = 0;
        continue_condition += optimization::o1 (func);
        continue_condition += optimization::o2 (func);

        fprintf (service::tex_file, "$ получим $");
        fprintf (service::tex_file, func);
        fprintf (service::tex_file, "$\n\n");
    }
    fprintf (service::tex_file, "После всех преобразований получили конечную формулу:\n\n");

    fprintf (service::tex_file, "\\begin{equation}\n");

    fprintf (service::tex_file, "    y'(x) = ");
    fprintf (service::tex_file, func);

    fprintf (service::tex_file, "\n\\end{equation}\n");

    return 0;
}


int optimization::o1 (tree_t* func)
{
    int is_edited = optimization::o1 (func, func -> root);
    return is_edited;
}


int optimization::o1 (tree_t* func, node* element)
{
    int is_edited = 0;
    if (element -> left && element -> right)
    {
        if (eltype == type::op_t && elop == oplus)
        {
            if (lefttype == type::number && leftnumber == 0)
            {
                if (element -> prev)
                {
                    if (element == element -> prev -> left)
                        tree_lconnect (func, element -> prev, element -> right);
                    else
                        tree_rconnect (func, element -> prev, element -> right);
                }
                else
                    tree_lconnect (func, 0, element -> right);

                is_edited = 1;
            }

            else if (righttype == type::number && rightnumber == 0)
            {
                if (element -> prev)
                {
                    if (element == element -> prev -> left)
                        tree_lconnect (func, element -> prev, element -> left);
                    else
                        tree_rconnect (func, element -> prev, element -> left);
                }

                else
                    tree_lconnect (func, 0, element -> left);

                is_edited = 1;
            }
        }

        else if (eltype == type::op_t && elop == omul)
        {
            if ((lefttype == type::number && leftnumber == 0) || (righttype == type::number && rightnumber == 0))
            {
                eltype = type::number;
                elnumber = 0;
                element -> left = 0;
                element -> right = 0;
                is_edited = 1;
            }

            else if (lefttype == type::number && leftnumber == 1)
            {
                if (element == element -> prev -> left)
                    tree_lconnect (func, element -> prev, element -> right);
                else
                    tree_rconnect (func, element -> prev, element -> right);
                is_edited = 1;
            }

            else if (righttype == type::number && rightnumber == 1)
            {
                if (element == element -> prev -> left)
                    tree_lconnect (func, element -> prev, element -> left);
                else
                    tree_rconnect (func, element -> prev, element -> left);
                is_edited = 1;
            }
        }
    }
    if (element -> left)
    {
        int is_edited_left = optimization::o1 (func, element -> left);
        is_edited += is_edited_left;
    }

    if (element -> right)
    {
        int is_edited_right = optimization::o1 (func, element -> right);
        is_edited += is_edited_right;
    }
    return is_edited;
}


int optimization::o2 (tree_t* func)
{
    int is_edited = optimization::o2 (func, func -> root);
    return is_edited;
}


int optimization::o2 (tree_t* func, node* element)
{
    int is_edited = 0;
    if (element -> left && element -> right)
    {
        if (eltype == type::op_t && elop == oplus && lefttype == type::number && righttype == type::number)
        {
            eltype = type::number;
            elnumber = leftnumber + rightnumber;
            element -> left = 0;
            element -> right = 0;
            is_edited = 1;
        }

    if (eltype == type::op_t && elop == omul && lefttype == type::number && righttype == type::number)
        {
            eltype = type::number;
            elnumber = leftnumber * rightnumber;
            element -> left = 0;
            element -> right = 0;
            is_edited = 1;
        }

    if (eltype == type::op_t && elop == ominus && lefttype == type::number && righttype == type::number)
        {
            eltype = type::number;
            elnumber = leftnumber - rightnumber;
            element -> left = 0;
            element -> right = 0;
            is_edited = 1;
        }

    if (eltype == type::op_t && elop == odiv && lefttype == type::number && righttype == type::number)
        {
            eltype = type::number;
            elnumber = leftnumber / rightnumber;
            element -> left = 0;
            element -> right = 0;
            is_edited = 1;
        }
    }

    if (element -> left)
    {
        int is_edited_left = optimization::o2 (func, element -> left);
        is_edited += is_edited_left;
    }

    if (element -> right)
    {
        int is_edited_right = optimization::o2 (func, element -> right);
        is_edited += is_edited_right;
    }
    return is_edited;
}


int print (tree_t* func)
{
    print (func -> root);
    return 0;
}


int print (node* element)
{
    if (element -> left)
    {
        printf ("(");
        print (element -> left);
    }

    if (eltype == type::number) printf ("%g", elnumber);
    if (eltype == type::constanta) printf ("%c", elconstanta);
    if (eltype == type::variable) printf ("%c", elvariable);
    if (eltype == type::op_t)
    {
        #define DEF_OP( operator, num, name, code )         \
                case num: printf ("%s", operator); break;   \

        switch (elop)
        {
            #include "operators.h"
            default: break;
        }

        #undef DEF_OP
    }

    if (element -> right)
    {
        print (element -> right);
        printf (")");
    }

    return 0;
}


int fprintf (FILE* file, tree_t* func)
{
    fprintf (file, func -> root);
    return 0;
}


int fprintf (FILE* file, node* element)
{
    if (element -> left)
    {
        fprintf (file, "(");
        fprintf (file, element -> left);
    }

    if (eltype == type::number) fprintf (file, "%g", elnumber);
    if (eltype == type::constanta) fprintf (file, "%c", elconstanta);
    if (eltype == type::variable) fprintf (file, "%c", elvariable);
    if (eltype == type::op_t)
    {
        #define DEF_OP( operator, num, name, code )                 \
                case num: fprintf (file, "%s", operator); break;    \

        switch (elop)
        {
            #include "operators.h"
            default: break;
        }

        #undef DEF_OP
    }

    if (element -> right)
    {
        fprintf (file, element -> right);
        fprintf (file, ")");
    }

    return 0;
}



int service::dump (tree_t* tree, char filename [], int mode)
{
    FILE* file = fopen ("graph_dump.txt", "w");
    fprintf (file, "digraph G{\n");
    node* element = tree -> root;
    switch (eltype)
        {
            case type::op_t:
            {
                #define DEF_OP( operator, num, name, code )         \
                        case num:                                   \
                        {                                           \
                            fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> ", element);  \
                            fprintf (file, operator);  \
                            fprintf (file, " | {<left> LEFT | <right> RIGHT}}\"]\n");   \
                            break;                                  \
                        }

                switch (elop)
                {
                    #include "operators.h"
                    default: break;
                }

                #undef DEF_OP

                break;
            }
            case type::number: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %g | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((func_element*) element -> data) -> number); break;
            case type::constanta: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %c | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((func_element*) element -> data) -> constanta); break;
            case type::variable: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %c | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((func_element*) element -> data) -> variable); break;
            default: break;
        }
    dump (element, file);
    fprintf (file, "}");
    fclose (file);
    char command [100] = "dot -Tpng graph_dump.txt -o ";
    strcat (command, filename);
    system (command);
    system ("del graph_dump.txt");
    if (mode) system (filename);
    return 0;
}


int service::dump (node* element, FILE* file)
{
    if (element -> left)
    {
        switch (lefttype)
        {
            case type::op_t:
            {
                #define DEF_OP( operator, num, name, code )         \
                        case num:                                   \
                        {                                           \
                            fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> ", element -> left);  \
                            fprintf (file, operator);  \
                            fprintf (file, " | {<left> LEFT | <right> RIGHT}}\"]\n");   \
                            break;                                  \
                        }

                switch (leftop)
                {
                    #include "operators.h"
                    default: break;
                }

                #undef DEF_OP

                break;
            }
            case type::number: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %g | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left, ((func_element*) element -> left -> data) -> number); break;
            case type::constanta: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %c | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left, ((func_element*) element -> left -> data) -> constanta); break;
            case type::variable: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %c | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left, ((func_element*) element -> left -> data) -> variable); break;
            default: break;
        }
        fprintf (file, "%d:<left> -> %d [color = blue]\n", element, element -> left);
        dump (element -> left, file);
    }

    if (element -> right)
    {
        switch (righttype)
        {
            case type::op_t:
            {
                #define DEF_OP( operator, num, name, code )         \
                        case num:                                   \
                        {                                           \
                            fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> ", element -> right);  \
                            fprintf (file, operator);  \
                            fprintf (file, " | {<left> LEFT | <right> RIGHT}}\"]\n");   \
                            break;                                  \
                        }

                switch (rightop)
                {
                    #include "operators.h"
                    default: break;
                }

                #undef DEF_OP

                break;
            }
            case type::number: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %g | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right, ((func_element*) element -> right -> data) -> number); break;
            case type::constanta: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %c | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right, ((func_element*) element -> right -> data) -> constanta); break;
            case type::variable: fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \" {<data> %c | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right, ((func_element*) element -> right -> data) -> variable); break;
            default: break;
        }
        fprintf (file, "%d:<right> -> %d [color = blue]\n", element, element -> right);
        dump (element -> right, file);
    }

    return 0;
}


//end
