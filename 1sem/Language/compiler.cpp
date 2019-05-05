///Шапитон
//цирк (гр)ёбаный

#include <cstdlib>
#include <fstream>
#include <cstring>
#include "tree_lib.cpp"

#include <assert.h>
#include "service.h"

#define id_max_length 50



static const int meow_operator = 265;
int program_length = 0;
int if_number = 0;



struct part
{
    int     type            = 0;
    int     op              = 0;
    double  number          = 0;
    int     identificator   = 0;
    int     is_func         = 0;
    int     var_count       = 0;
    int     mem_size        = 0;
};


struct id
{
    char name [id_max_length] = {};
    int number = 0;
};


namespace type
{
    const int corrupted_element = -1;
    const int op_node = 1;
    const int def_node = 2;
    const int op = 3;
    const int number = 4;
    const int identificator = 5;
    const int assignment = 6;
}


namespace functype
{
    const int not_a_function = 0;
    const int declaration = 1;
    const int call = 2;
}


namespace global
{
    char file_name [50] = {};
}


int identificators_len = 0;
id* identificators = 0;
int* id_numeration = 0;


#define DEF_OP( name, operator, number, code )    \
        operator_##name = number,

enum operators
{
    #include "operators.h"
    operators_count
};

#undef DEF_OP




char*   open_program ();
long    file_size   (FILE* file);


int     get_length  (char** buffer, int* cmd_count, int* id_count);
int     get_id      (const char current_id []);
node**  analize     (char** buffer);


tree_t  build_tree  (node** program_text_analized);

namespace grammatic
{
    node* GetG0 	(tree_t* program_tree, node** program_text_analized, int* pointer);

    node* GetDef 	(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetOp 	(tree_t* program_tree, node** program_text_analized, int* pointer);

    node* GetStdFunc(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetAssgn 	(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetWhile 	(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetFor 	(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetIf 	(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetElse 	(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetCall   (tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetRet    (tree_t* program_tree, node** program_text_analized, int* pointer);

    node* GetItemize(tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetLength (tree_t* program_tree, node** program_text_analized, int* pointer);

    node* GetC      (tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetE 	    (tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetT 	    (tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetF 	    (tree_t* program_tree, node** program_text_analized, int* pointer);
    node* GetP 	    (tree_t* program_tree, node** program_text_analized, int* pointer);
}


int     compile         (tree_t* program_tree);
int     compile         (node* element, FILE* compiled_code);
int     compile_assgn   (node* element, FILE* compiled_code);
int     compile_def     (node* element, FILE* compiled_code);
int     compile_return  (node* element, FILE* compiled_code);
int     compile_call    (node* element, FILE* compiled_code);
int     compile_while   (node* element, FILE* compiled_code);
int     compile_if      (node* element, FILE* compiled_code);
int     compile_input   (node* element, FILE* compiled_code);


namespace optimization
{
    int optimize    (tree_t* func);
    int o1          (tree_t* func);
    int o1          (tree_t* func, node* element);
    int o2          (tree_t* func);
    int o2          (tree_t* func, node* element);
    int o3          (tree_t* func);
    int o3          (tree_t* func, node* element);
}


int renumerate (int initial_number);
int reset_numeration ();
int memory_allocation (tree_t* code);
int memory_allocation (node* element);

int main_variables_count = 0;
node* last_def = 0;


namespace service
{
    const int HIDDEN = 0;
    const int SHOW = 1;
    int dump (tree_t* tree, char filename [], int mode);
    int dump (node* element, char filename [], int mode);
    int dump (node* element, FILE* file);
}




#include "defines.h"




int main (int argc, const char* argv [])
{
    if (argc >= 2) strcpy (global::file_name, argv [1]);
    else
    {
        printf ("Enter name of file with program: ");
        scanf ("%s", &global::file_name);
    }

    char* program_text = open_program ();

    node** program_text_analized = analize (&program_text);
    //for (int counter = 0; counter <= program_length; counter++)
        //printf ("type %d, operator %d, number %lg, identificator %d\n", ((part*) program_text_analized [counter] -> data) -> type, ((part*) program_text_analized [counter] -> data) -> op, ((part*) program_text_analized [counter] -> data) -> number, ((part*) program_text_analized [counter] -> data) -> identificator);

    tree_t program_tree = build_tree (program_text_analized);


    compile (&program_tree);

    service::dump (&program_tree, "program.png", service::SHOW);

    //system ("compiled_code.myasm");

    return 0;
}




char* open_program ()
{
    FILE* program_file = fopen (global::file_name, "r");
    long file_length = file_size (program_file);

    char* program_text = (char*) calloc (sizeof (char), file_length + 1);
    fread (program_text, sizeof (char), file_length, program_file);
    program_text [file_length] = 3; //3 = '[end of text]' in ASKII

    return program_text;
}

long file_size (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long file_size = ftell (file);
    rewind (file);

    return file_size;
}




int get_id (const char current_id [])
{
    int is_declared = 0;
    int id_number = 0;

    for (int counter = 0; counter < identificators_len; counter++)
    {
        if (!strcmp ((identificators [counter]).name, current_id))
        {
            is_declared = 1;
            id_number = (identificators [counter]).number;
            return id_number;
        }
    }

    if (!is_declared)
    {
        strcpy ((identificators [identificators_len]).name, current_id);
        (identificators [identificators_len]).number = identificators_len + 1;
        return (++identificators_len);
    }

    return -1;
}

int get_length (char** buffer, int* cmd_count, int* id_count)
{
    const int nothing = 0;
    const int number = 1;
    const int word = 2;
    const int sym = 3;
    const int unknown_symbol = -1;

    int automat_state = nothing;
    int reading_ended = 0;

    int counter = 0;

    while ((*buffer) [counter] != 3)
    {
        if (automat_state == nothing)
        {
            if (issym ((*buffer) [counter]))
                automat_state = sym;

            else if (isnumber ((*buffer) [counter]))
                automat_state = number;

            else if (isletter ((*buffer) [counter]))
                automat_state = word;

            else
                automat_state = unknown_symbol;
        }

        if (automat_state == number)
        {
            if (isnumber ((*buffer) [counter]))
                counter++;
            else
                reading_ended = 1;
        }

        if (automat_state == word)
        {
            if (isnumber ((*buffer) [counter]) || isletter ((*buffer) [counter]))
                counter++;
            else
                reading_ended = 1;
        }

        if (automat_state == sym)
        {
            if (issym ((*buffer) [counter]))
            {
                if (isbracket ((*buffer) [counter]))
                    reading_ended = 1;

                counter++;
            }

            else
                reading_ended = 1;
        }

        if (automat_state == unknown_symbol)
        {
            counter++;
            automat_state = 0;
        }

        if (reading_ended)
        {
            (*cmd_count)++;
            if (automat_state == word)
                (*id_count)++;

            automat_state = 0;
            reading_ended = 0;
        }
    }

    program_length = *cmd_count;

    return 0;
}

node** analize (char** buffer)
{
    int id_count = 0;
    int cmd_count = 0;
    get_length (buffer, &cmd_count, &id_count);
    node** analized_program = (node**) calloc (sizeof (node*), cmd_count);

    identificators = (id*) calloc (sizeof (id), id_count);
    id_numeration = (int*) calloc (sizeof (int), id_count);

    long program_length = 0;
    int counter = 0;

    int line_number = 1;
    int symbol_number = 1;

    int max_operator_length = 50;

    const int nothing = 0;
    const int number = 1;
    const int word = 2;
    const int sym = 3;
    const int unknown_symbol = -1;
    const int unknown_operator = -1;

    int automat_state = nothing;
    int reading_ended = 0;

    char element [max_operator_length] = {};
    int element_counter = 0;

    while ((*buffer) [counter - 1] != 3)
    {
        symbol_number++;
        if ((*buffer) [counter] == '\n')
        {
            line_number++;
            symbol_number = 0;
        }

        //Конечный автомат (как я)
        if (automat_state == nothing)
        {
            if (issym ((*buffer) [counter]))
                automat_state = sym;

            else if (isnumber ((*buffer) [counter]))
                automat_state = number;

            else if (isletter ((*buffer) [counter]))
                automat_state = word;

            else
                automat_state = unknown_symbol;
        }

        if (automat_state == number)
        {
            if (isnumber ((*buffer) [counter]))
                element [element_counter++] = (*buffer) [counter++];
            else
                reading_ended = 1;
        }

        if (automat_state == word)
        {
            if (isnumber ((*buffer) [counter]) || isletter ((*buffer) [counter]))
                element [element_counter++] = (*buffer) [counter++];
            else
                reading_ended = 1;
        }

        if (automat_state == sym)
        {
            if (issym ((*buffer) [counter]))
            {
                element [element_counter] = (*buffer) [counter];
                if (isbracket ((*buffer) [counter]))
                    reading_ended = 1;

                element_counter++;
                counter++;
            }

            else
                reading_ended = 1;
        }

        if (automat_state == unknown_symbol)
        {
            counter++;
            automat_state = 0;
        }

        if (reading_ended)
        {
            part particical;

            if (automat_state == number)
            {
                element [element_counter] = '\0';

                particical.type = type::number;
                particical.number = atof (element);
            }

            else if (automat_state == word)
            {
                element [element_counter] = '\0';

                if (!strcmp (element, "meow"))
                {
                    particical.type = type::op;
                    particical.op = meow_operator;
                }

                #define DEF_OP( name, operator, number, code )              \
                        else if (!strcmp (operator, element))               \
                        {                                                   \
                            particical.type = type::op;                     \
                            particical.op = number;                         \
                        }

                #include "operators.h"

                #undef DEF_OP

                else
                {
                    particical.type = type::identificator;
                    particical.identificator = get_id (element);
                }
            }

            else if (automat_state == sym)
            {
                element [element_counter] = '\0';

                if (!strcmp (element, "meow"))
                {
                    particical.type = type::op;
                    particical.op = meow_operator;
                }

                #define DEF_OP( name, operator, number, code )              \
                        else if (!strcmp (operator, element))               \
                        {                                                   \
                            particical.type = type::op;                     \
                            particical.op = number;                         \
                        }

                #include "operators.h"

                #undef DEF_OP

                else
                {
                    particical.type = type::op;
                    particical.op = unknown_operator;
                    printf ("Error in line %d in %d symbol: Unknown operator \"%s\"\n", line_number, symbol_number, element);
                }
            }

            else
                printf ("Error in line %d in %d symbol: Unknown type\n", line_number, symbol_number);


            element_counter = 0;
            automat_state = 0;
            reading_ended = 0;

            printf ("now readed %s, type %d, operator %d, number %lg, identificator %d\n", element, particical.type, particical.op, particical.number, particical.identificator);

            node* new_node = create_node (particical);
            analized_program [program_length++] = new_node;
        }
    }

    return analized_program;
}




tree_t build_tree (node** program_text_analized)
{
    int pointer = 0;
    tree_t program_tree;

    program_tree.root = grammatic::GetG0 (&program_tree, program_text_analized, &pointer);

    tree_fix_prev (&program_tree);

    return program_tree;
}




node* grammatic::GetG0 (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = 0;
    node* new_branch = 0;

    int met_type = 0;
    int met_op = 0;

    if (*pointer < program_length)
    {
        met_type = eltype;
        met_op = elop;
    }

    if (met_type == type::op && (met_op == operator_def || met_op == operator_ocode))
    {
        while (*pointer < program_length)
        {
            new_branch = _GetDef;

            if (branch)
                branch = add_def (new_branch, branch);

            else
                branch = new_branch;

            if (*pointer < program_length)
            {
                met_type = eltype;
                met_op = elop;
            }

            else
            {
                met_type = 0;
                met_op = 0;
            }
        }
    }

    return branch;
}




node* grammatic::GetDef (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    //NOT_IMPLEMENTED

    if (!(eltype == type::op && elop == operator_def))
        return _GetOp;

    (*pointer)++;

    node* name = (program_text_analized) [*pointer];

    elfunc = functype::declaration;

    (*pointer)++;

    node* left_branch = _GetItemize;

    node* right_branch = _GetOp;

    return create_def (name, left_branch, right_branch);
}

node* grammatic::GetOp (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    part op_node;
    op_node.type = type::op_node;

    node* branch = 0;
    node* new_branch = 0;

    int met_type = 0;
    int met_op = 0;

    if (*pointer < program_length)
    {
        met_type = eltype;
        met_op = elop;
    }

    if (met_type == type::op && met_op == operator_ocode)
    {
        (*pointer)++;

        while (!(met_type == type::op && met_op == operator_clcode))
        {
            new_branch = _GetOp;

            if (branch)
                branch = add_op (branch, new_branch);

            else
                branch = new_branch;

            if (*pointer < program_length)
            {
                met_type = eltype;
                met_op = elop;
            }

            else
            {
                met_type = 0;
                met_op = 0;
            }
        }

        assert (eltype == type::op && elop == operator_clcode);

        (*pointer)++;
    }

    else
    {
        if (eltype == type::op && elop == operator_while)
            branch = _GetWhile;

        else if (eltype == type::op && elop == operator_for)
            branch = _GetFor;

        else if (eltype == type::op && elop == operator_if)
            branch = _GetIf;

        else if (eltype == type::op && elop == operator_else)
            branch = _GetElse;

        else if (eltype == type::op && elop == operator_return)
            branch = _GetRet;

        else if (eltype == type::identificator && nexttype == type::op && nextop == operator_assgn)
            branch = _GetAssgn;

        else if (eltype == type::identificator && nexttype == type::op && nextop == operator_obracket)
            branch = _GetCall;

        else if (eltype == type::op)
            branch = _GetStdFunc;

        else
            printf ("syntax error\n");
    }

    return branch;
}




node* grammatic::GetStdFunc (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = (program_text_analized) [*pointer];
    (*pointer)++;
    branch -> left = _GetItemize;
    return branch;
}


node* grammatic::GetAssgn (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* left_branch = _GetP;

    (*pointer)++;

    node* right_branch = _GetE;

    return add_assgn (left_branch, right_branch);
}

node* grammatic::GetWhile (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    (*pointer)++;

    node* branch_left = _GetC;

    node* branch_right = _GetOp;

    return add_while (branch_left, branch_right);
}

node* grammatic::GetFor (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    NOT_IMPLEMENTED
    return 0;
}

node* grammatic::GetIf (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    (*pointer)++;

    node* branch_left = _GetC;

    node* branch_right = _GetOp;

    return add_if (branch_left, branch_right);
}

node* grammatic::GetElse (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    NOT_IMPLEMENTED
    return 0;
}

node* grammatic::GetCall (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* name = program_text_analized [*pointer];

    (*pointer)++;

    if (eltype == type::op && elop == operator_obracket)
        ((part*) name -> data) -> is_func = functype::call;

    node* parameters = _GetItemize;

    return create_def (name, parameters, 0);
}

node* grammatic::GetRet (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = (program_text_analized) [*pointer];
    (*pointer)++;

    branch -> left = (program_text_analized) [*pointer];
    (*pointer)++;

    return branch;
}




node* grammatic::GetItemize (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    if (!(eltype == type::op && elop == operator_obracket))
        return 0;

    node* branch = 0;

    (*pointer)++;

    while (!(eltype == type::op && elop == operator_clbracket))
    {
        if (!branch)
            branch = program_text_analized [*pointer];

        else
        {
            program_text_analized [*pointer] -> left = branch;
            branch = program_text_analized [*pointer];
        }


        (*pointer)++;

        if (!(eltype == type::op && elop == operator_itemize))
            break;

        (*pointer)++;
    }

    assert (eltype == type::op && elop == operator_clbracket);

    (*pointer)++;

    return branch;
}

node* grammatic::GetLength (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    NOT_IMPLEMENTED

    return 0;
}




node* grammatic::GetC (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = _GetE;

    int met_type = 0;
    int met_op = 0;

    node* branch_right = 0;

    if (*pointer < program_length)
    {
        met_type = eltype;
        met_op = elop;
    }

    while (met_type == type::op && iscompare (met_op))
    {
        (*pointer)++;
        branch_right = _GetE;

        if (met_op == operator_more)
            branch = *branch_right > *branch;

        else if (met_op == operator_moreeq)
            branch = *branch_right >= *branch;

        else if (met_op == operator_less)
            branch = *branch_right < *branch;

        else if (met_op == operator_lesseq)
            branch = *branch_right <= *branch;

        else if (met_op == operator_equal)
            branch = *branch_right == *branch;

        if (*pointer < program_length)
        {
            met_type = eltype;
            met_op = elop;
        }

        else
        {
            met_type = 0;
            met_op = 0;
        }
    }

    return branch;
}

node* grammatic::GetE (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = _GetT;

    int met_type = 0;
    int met_op = 0;

    node* branch_right = 0;

    if (*pointer < program_length)
    {
        met_type = eltype;
        met_op = elop;
    }

    while (met_type == type::op && (met_op == operator_plus || met_op == operator_minus))
    {
        (*pointer)++;
        branch_right = _GetT;

        if (met_op == operator_plus)
            branch = *branch_right + *branch;

        else
            branch = *branch_right - *branch;

        if (*pointer < program_length)
        {
            met_type = eltype;
            met_op = elop;
        }

        else
        {
            met_type = 0;
            met_op = 0;
        }
    }

    return branch;
}

node* grammatic::GetT (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = _GetF;

    int met_type = 0;
    int met_op = 0;

    if (*pointer < program_length)
    {
        met_type = eltype;
        met_op = elop;
    }

    while (met_type == type::op && (met_op == operator_mul || met_op == operator_div))
    {
        (*pointer)++;
        node* branch_right = _GetF;

        if (met_op == operator_mul)
            branch = *branch_right * *branch;

        if (met_op == operator_div)
            branch = *branch_right / *branch;

        if (*pointer < program_length)
        {
            met_type = eltype;
            met_op = elop;
        }

        else
        {
            met_type = 0;
            met_op = 0;
        }
    }

    return branch;
}

node* grammatic::GetF (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = _GetP;

    int met_type = 0;
    int met_op = 0;

    if (*pointer < program_length)
    {
        met_type = eltype;
        met_op = elop;
    }

    while (met_type == type::op && met_op == operator_power)
    {
        (*pointer)++;
        node* branch_right = _GetP;
        branch = *branch_right ^ *branch;

        if (*pointer < program_length)
        {
            met_type = eltype;
            met_op = elop;
        }

        else
        {
            met_type = 0;
            met_op = 0;
        }
    }

    return branch;
}

node* grammatic::GetP (tree_t* program_tree, node** program_text_analized, int* pointer)
{
    node* branch = 0;
    if (eltype == type::op && elop == operator_obracket)
    {
        (*pointer)++;
        branch = _GetC;
        assert (eltype == type::op && elop == operator_clbracket);
        (*pointer)++;
    }

    else if (eltype == type::number)
    {
        branch = program_text_analized [*pointer];
        (*pointer)++;
    }

    else if (eltype == type::identificator)
    {
        branch = _GetCall;
    }

    else
        printf ("Sintax error! %d %d %g %d\n", eltype, elop, elnumber, elidentificator);

    return branch;
}







int compile (tree_t* program_tree)
{
    optimization::optimize (program_tree);

    memory_allocation (program_tree);

    FILE* compiled_code = fopen ("compiled_code.myasm", "w");

    fprintf (compiled_code, "KV_1.0\n");

    ///INDENTATION INIT///

    fprintf (compiled_code, "#cx - регистр с будущим отступом\n");
    fprintf (compiled_code, "#dx - регистр с текущим отступом\n");
    fprintf (compiled_code, "push 0\n");
    fprintf (compiled_code, "pop dx\n");
    fprintf (compiled_code, "push %d\n", main_variables_count);
    fprintf (compiled_code, "pop cx\n");


    compile (program_tree -> root, compiled_code);

    fprintf (compiled_code, "end\n");

    fclose (compiled_code);

    return 0;
}


///OLD COMPILATION BLOCK///


int compile (node* element, FILE* compiled_code)
{
    if (elemtype == type::op && elemop == operator_assgn)
    {
        compile_assgn (element, compiled_code);
        return 0;
    }

    if (elemtype == type::identificator && elemfunc == functype::declaration)
    {
        fprintf (compiled_code, "end\n");
        compile_def (element, compiled_code);
        return 0;
    }

    if (elemtype == type::op && elemop == operator_return)
    {
        compile_return (element, compiled_code);
        return 0;
    }

    if (elemtype == type::identificator && elemfunc == functype::call)
    {
        compile_call (element, compiled_code);
        return 0;
    }

    if (elemtype == type::op && elemop == operator_while)
    {
        compile_while (element, compiled_code);
        return 0;
    }

    if (elemtype == type::op && elemop == operator_if)
    {
        compile_if (element, compiled_code);
        return 0;
    }

    if (elemtype == type::op && elemop == operator_input)
    {
        compile_input (element, compiled_code);
        return 0;
    }

    if (element -> left)
        compile (element -> left, compiled_code);

    if (element -> right)
        compile (element -> right, compiled_code);

    switch (elemtype)
    {
        case type::number: fprintf (compiled_code, "push %g\n", elemnumber); break;
        case type::identificator:
        {
            fprintf (compiled_code, "push %d\n", elemidentificator);
            fprintf (compiled_code, "push dx\n");
            fprintf (compiled_code, "add\n");
            fprintf (compiled_code, "pop ax\n");
            fprintf (compiled_code, "push [ax]\n", elemidentificator);
            break;
        }
        case type::op:
        {
            #define DEF_OP( name, operator, num, code )       \
                    case num: code; break;

            switch (elemop)
            {
                #include "operators.h"
            }

            #undef DEF_OP
        }
    }

    return 0;
}


int compile_assgn (node* element, FILE* compiled_code)
{
    assert (elemtype == type::op && elemop == operator_assgn);

    fprintf (compiled_code, "push %d\n", leftidentificator);
    fprintf (compiled_code, "push dx\n");
    fprintf (compiled_code, "add\n");

    compile (element -> right, compiled_code);

    fprintf (compiled_code, "pop bx\n");
    fprintf (compiled_code, "pop ax\n");
    fprintf (compiled_code, "push bx\n");
    fprintf (compiled_code, "pop [ax]\n");

    return 0;
}


int compile_def (node* element, FILE* compiled_code)
{
    fprintf (compiled_code, "\n#function\n:func%d\n", elemidentificator);
    fprintf (compiled_code, "push cx\n");
    fprintf (compiled_code, "push %d\n", elemvcount);
    fprintf (compiled_code, "add\n");
    fprintf (compiled_code, "pop cx\n");

    node* parameter = element -> left;

    while (parameter)
    {
        fprintf (compiled_code, "push %d\n", ((part*) parameter -> data) -> identificator);
        fprintf (compiled_code, "push dx\n");
        fprintf (compiled_code, "add\n");
        fprintf (compiled_code, "pop ax\n");
        fprintf (compiled_code, "push [ax]\n");
        parameter = parameter -> left;
    }

    if (element -> right)
        compile (element -> right, compiled_code);

    fprintf (compiled_code, "push %d\n", elemvcount);
    fprintf (compiled_code, "push cx\n");
    fprintf (compiled_code, "sub\n");
    fprintf (compiled_code, "pop cx\n");
    fprintf (compiled_code, "#end of function\n");

    return 0;
}


int compile_return (node* element, FILE* compiled_code)
{
    assert (elemtype == type::op && elemop == operator_return);

    node* parameter = element -> left;

    if (!parameter)
        assert (!"finded return without parameters!");

    if (parameter)
    {
        fprintf (compiled_code, "push %d\n", ((part*) parameter -> data) -> identificator);
        fprintf (compiled_code, "push dx\n");
        fprintf (compiled_code, "add\n");
        fprintf (compiled_code, "pop ax\n");
        fprintf (compiled_code, "push [ax]\n");
        fprintf (compiled_code, "pop bx\n");
    }

    fprintf (compiled_code, "ret\n");

    return 0;
}


int compile_call (node* element, FILE* compiled_code)
{
    node* parameter = element -> left;

    fprintf (compiled_code, "push dx\n");

    while (parameter)
    {
        fprintf (compiled_code, "push %d\n", ((part*) parameter -> data) -> identificator);
        fprintf (compiled_code, "push dx\n");
        fprintf (compiled_code, "add\n");
        fprintf (compiled_code, "pop ax\n");
        fprintf (compiled_code, "push [ax]\n");
        parameter = parameter -> left;
    }

    fprintf (compiled_code, "push cx\n");
    fprintf (compiled_code, "pop dx\n");
    fprintf (compiled_code, "call :func%d\n", ((part*) element -> data) -> identificator);
    fprintf (compiled_code, "pop dx\n");
    fprintf (compiled_code, "push bx\n");

    return 0;
}


int compile_if (node* element, FILE* compiled_code)
{
    if (lefttype == type::op && iscompare (leftop))
    {
        if (leftop == operator_more)
        {
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jbe :if%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, ":if%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_moreeq)
        {
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jb :if%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, ":if%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_less)
        {
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jae :if%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, ":if%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_lesseq)
        {
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "ja :if%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, ":if%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_equal)
        {
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jne :if%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, ":if%d\n", if_number);
            if_number++;
        }
    }

    else
    {
        fprintf (compiled_code, "push 0\n");
        compile (element -> left, compiled_code);
        fprintf (compiled_code, "je :if%d\n", if_number);
        compile (element -> right, compiled_code);
        fprintf (compiled_code, ":if%d\n", if_number);
        if_number++;
    }

    return 0;
}


int compile_while (node* element, FILE* compiled_code)
{
    if (lefttype == type::op && iscompare (leftop))
    {
        if (leftop == operator_more) //jbe
        {
            fprintf (compiled_code, ":whilestart%d\n", if_number);
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jbe :whileexit%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, "jmp :whilestart%d\n", if_number);
            fprintf (compiled_code, ":whileexit%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_moreeq) //jb
        {
            fprintf (compiled_code, ":whilestart%d\n", if_number);
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jb :whileexit%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, "jmp :whilestart%d\n", if_number);
            fprintf (compiled_code, ":whileexit%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_less) //jae
        {
            fprintf (compiled_code, ":whilestart%d\n", if_number);
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jae :whileexit%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, "jmp :whilestart%d\n", if_number);
            fprintf (compiled_code, ":whileexit%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_lesseq) //ja
        {
            fprintf (compiled_code, ":whilestart%d\n", if_number);
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "ja :whileexit%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, "jmp :whilestart%d\n", if_number);
            fprintf (compiled_code, ":whileexit%d\n", if_number);
            if_number++;
        }

        else if (leftop == operator_equal) //jne
        {
            fprintf (compiled_code, ":whilestart%d\n", if_number);
            compile (element -> left -> left, compiled_code);
            compile (element -> left -> right, compiled_code);
            fprintf (compiled_code, "jne :whileexit%d\n", if_number);
            compile (element -> right, compiled_code);
            fprintf (compiled_code, "jmp :whilestart%d\n", if_number);
            fprintf (compiled_code, ":whileexit%d\n", if_number);
            if_number++;
        }
    }

    else
    {
        fprintf (compiled_code, ":whilestart%d\n", if_number);
        fprintf (compiled_code, "push 0\n", if_number);
        compile (element -> left, compiled_code);
        fprintf (compiled_code, "je :whileexit%d\n", if_number);
        compile (element -> right, compiled_code);
        fprintf (compiled_code, "jmp :whilestart%d\n", if_number);
        fprintf (compiled_code, ":whileexit%d\n", if_number);
        if_number++;
    }

    return 0;
}


int compile_input (node* element, FILE* compiled_code)
{
    assert (elemtype == type::op && elemop == operator_input);

    fprintf (compiled_code, "in\n");
    fprintf (compiled_code, "push %d\n", leftidentificator);
    fprintf (compiled_code, "push dx\n");
    fprintf (compiled_code, "add\n");
    fprintf (compiled_code, "pop ax\n");
    fprintf (compiled_code, "pop [ax]\n");

    return 0;
}


///OLD COMPILATION BLOCK END///



int renumerate (int initial_number)
{
    for (int counter = 1; counter <= id_numeration [0]; counter++)
    {
        if (id_numeration [counter] == initial_number)
            return counter;
    }

    int new_position = id_numeration [0] + 1;
    id_numeration [new_position] = initial_number;
    id_numeration [0] = new_position;

    return id_numeration [0];
}


int reset_numeration ()
{
    int count_of_variables = id_numeration [0];
    id_numeration [0] = 0;
    return count_of_variables;
}


int memory_allocation (tree_t* code)
{
    memory_allocation (code -> root);
    return 0;
}


int memory_allocation (node* element)
{
    if (elemtype == type::identificator && elemfunc == functype::declaration)
    {
        int count_of_variables = reset_numeration ();

        if (!main_variables_count)
            main_variables_count = count_of_variables;

        if (last_def)
        {
            ((part*) last_def -> data) -> var_count = count_of_variables;
        }

        last_def = element;
    }

    if (element -> left)
        memory_allocation (element -> left);

    if (element -> right)
        memory_allocation (element -> right);

    if (elemtype == type::identificator && elemfunc == functype::not_a_function)
        elemidentificator = renumerate (elemidentificator);

    return 0;
}


int optimization::optimize (tree_t* func)
{
    int continue_condition = 1;

    while (continue_condition)
    {
        continue_condition = 0;
        continue_condition += optimization::o1 (func);
        continue_condition += optimization::o2 (func);
    }

    //optimization::o3 (func);

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
        if (elemtype == type::op && elemop == operator_plus)
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

        else if (elemtype == type::op && elemop == operator_mul)
        {
            if ((lefttype == type::number && leftnumber == 0) || (righttype == type::number && rightnumber == 0))
            {
                elemtype = type::number;
                elemnumber = 0;
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
        if (elemtype == type::op && elemop == operator_plus && lefttype == type::number && righttype == type::number)
        {
            elemtype = type::number;
            elemnumber = leftnumber + rightnumber;
            element -> left = 0;
            element -> right = 0;
            is_edited = 1;
        }

    if (elemtype == type::op && elemop == operator_mul && lefttype == type::number && righttype == type::number)
        {
            elemtype = type::number;
            elemnumber = leftnumber * rightnumber;
            element -> left = 0;
            element -> right = 0;
            is_edited = 1;
        }

    if (elemtype == type::op && elemop == operator_minus && lefttype == type::number && righttype == type::number)
        {
            elemtype = type::number;
            elemnumber = leftnumber - rightnumber;
            element -> left = 0;
            element -> right = 0;
            is_edited = 1;
        }

    if (elemtype == type::op && elemop == operator_div && lefttype == type::number && righttype == type::number)
        {
            elemtype = type::number;
            elemnumber = leftnumber / rightnumber;
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


int optimization::o3 (tree_t* func)
{
    NOT_IMPLEMENTED
    int is_edited = optimization::o3 (func, func -> root);
    return is_edited;
}


int optimization::o3 (tree_t* func, node* element)
{
    NOT_IMPLEMENTED
    if (elemtype == type::op && (elemop == operator_if || elemop == operator_while))
    {
        if (lefttype == type::number && leftnumber == 0)
        {
            elemtype = type::op_node;
            elemnumber = 0;
            element -> left = 0;
            element -> right = 0;
        }

        if (lefttype == type::number && leftnumber != 0)
        {
            elemtype = type::op_node;
            elemnumber = 0;
            element -> left = 0;
        }
    }

    if (element -> left)
        optimization::o3 (func, element -> left);

    if (element -> right)
        optimization::o3 (func, element -> right);

    return 0;
}






int service::dump (node* element, char filename [], int mode)
{
    if (!element)
    {
        printf ("Error when try show empty branch!\n");
        return 0;
    }
    FILE* file = fopen ("graph_dump.txt", "w");
    fprintf (file, "digraph G{\n");
    switch (elemtype)
        {
            case type::op:
            {
                #define DEF_OP( name, operator, number, code )         \
                        case number:                                   \
                        {                                           \
                            fprintf (file, "%d [shape = record, style = filled, fillcolor = violet, color = black, label = \" {<data> ", element);  \
                            fprintf (file, operator);  \
                            fprintf (file, " | {<left> LEFT | <right> RIGHT}}\"]\n");   \
                            break;                                  \
                        }

                switch (elemop)
                {
                    #include "operators.h"
                    default: break;
                }

                #undef DEF_OP

                break;
            }
            case type::def_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = blue, color = black, label = \" {<data> DEF_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;
            case type::op_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = green, color = black, label = \" {<data> OP_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;
            case type::number: fprintf (file, "%d [shape = record, style = filled, fillcolor = cyan, color = black, label = \" {<data> %g | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> number); break;
            case type::identificator:
            {
                if (elemfunc == functype::not_a_function) fprintf (file, "%d [shape = record, style = filled, fillcolor = lightgray, color = black, label = \" {<data> memory: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> identificator);
                if (elemfunc == functype::declaration) fprintf (file, "%d [shape = record, style = filled, fillcolor = orange, color = black, label = \" {<data> function: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> identificator);
                if (elemfunc == functype::call) fprintf (file, "%d [shape = record, style = filled, fillcolor = yellow, color = black, label = \" {<data> func_call: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> identificator);
                break;
            }
            default: fprintf (file, "%d [shape = record, style = filled, fillcolor = red, color = white, label = \" {<data> UNCKNOWN (wtf) | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;;
        }
    dump (element, file);
    fprintf (file, "}");
    fclose (file);
    char command [100] = "dot -Tpng graph_dump.txt -o ";
    strcat (command, filename);
    system (command);
    //system ("del graph_dump.txt");
    if (mode) system (filename);
    return 0;
}


int service::dump (tree_t* tree, char filename [], int mode)
{
    if (!tree -> root)
    {
        printf ("Error when try show empty branch!\n");
        return 0;
    }
    FILE* file = fopen ("graph_dump.txt", "w");
    fprintf (file, "digraph G{\n");
    node* element = tree -> root;
    switch (elemtype)
        {
            case type::op:
            {
                #define DEF_OP( name, operator, number, code )         \
                        case number:                                   \
                        {                                           \
                            fprintf (file, "%d [shape = record, style = filled, fillcolor = violet, color = black, label = \" {<data> ", element);  \
                            fprintf (file, operator);  \
                            fprintf (file, " | {<left> LEFT | <right> RIGHT}}\"]\n");   \
                            break;                                  \
                        }

                switch (elemop)
                {
                    #include "operators.h"
                    default: break;
                }

                #undef DEF_OP

                break;
            }
            case type::def_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = blue, color = black, label = \" {<data> DEF_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;
            case type::op_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = green, color = black, label = \" {<data> OP_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;
            case type::number: fprintf (file, "%d [shape = record, style = filled, fillcolor = cyan, color = black, label = \" {<data> %g | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> number); break;
            case type::identificator:
            {
                if (elemfunc == functype::not_a_function) fprintf (file, "%d [shape = record, style = filled, fillcolor = lightgray, color = black, label = \" {<data> memory: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> identificator);
                if (elemfunc == functype::declaration) fprintf (file, "%d [shape = record, style = filled, fillcolor = orange, color = black, label = \" {<data> function: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> identificator);
                if (elemfunc == functype::call) fprintf (file, "%d [shape = record, style = filled, fillcolor = yellow, color = black, label = \" {<data> func_call: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element, ((part*) element -> data) -> identificator);
                break;
            }
            default: fprintf (file, "%d [shape = record, style = filled, fillcolor = red, color = white, label = \" {<data> UNCKNOWN (wtf) | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;;
        }
    dump (element, file);
    fprintf (file, "}");
    fclose (file);
    char command [100] = "dot -Tpng graph_dump.txt -o ";
    strcat (command, filename);
    system (command);
    //system ("del graph_dump.txt");
    if (mode) system (filename);
    return 0;
}


int service::dump (node* element, FILE* file)
{
    if (element -> left)
    {
        switch (lefttype)
        {
            case type::op:
            {
                #define DEF_OP( name, operator, number, code )         \
                        case number:                                   \
                        {                                           \
                            fprintf (file, "%d [shape = record, style = filled, fillcolor = violet, color = black, label = \" {<data> ", element -> left);  \
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
            case type::def_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = blue, color = black, label = \" {<data> DEF_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left); break;
            case type::op_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = green, color = black, label = \" {<data> OP_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left); break;
            case type::number: fprintf (file, "%d [shape = record, style = filled, fillcolor = cyan, color = black, label = \" {<data> %g | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left, ((part*) element -> left -> data) -> number); break;
            case type::identificator:
            {
                if (leftfunc == functype::not_a_function) fprintf (file, "%d [shape = record, style = filled, fillcolor = lightgray, color = black, label = \" {<data> memory: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left, ((part*) element -> left -> data) -> identificator);
                if (leftfunc == functype::declaration) fprintf (file, "%d [shape = record, style = filled, fillcolor = orange, color = black, label = \" {<data> function: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left, ((part*) element -> left -> data) -> identificator);
                if (leftfunc == functype::call) fprintf (file, "%d [shape = record, style = filled, fillcolor = yellow, color = black, label = \" {<data> func_call: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element -> left, ((part*) element -> left -> data) -> identificator);
                break;
            }
            default: fprintf (file, "%d [shape = record, style = filled, fillcolor = red, color = white, label = \" {<data> UNCKNOWN (wtf) | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;;
        }
        fprintf (file, "%d:<left> -> %d [color = black]\n", element, element -> left);
        dump (element -> left, file);
    }

    if (element -> right)
    {
        switch (righttype)
        {
            case type::op:
            {
                #define DEF_OP( name, operator, number, code )         \
                        case number:                                   \
                        {                                           \
                            fprintf (file, "%d [shape = record, style = filled, fillcolor = violet, color = black, label = \" {<data> ", element -> right);  \
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
            case type::def_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = blue, color = black, label = \" {<data> DEF_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right); break;
            case type::op_node: fprintf (file, "%d [shape = record, style = filled, fillcolor = green, color = black, label = \" {<data> OP_NODE | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right); break;
            case type::number: fprintf (file, "%d [shape = record, style = filled, fillcolor = cyan, color = black, label = \" {<data> %g | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right, ((part*) element -> right -> data) -> number); break;
            case type::identificator:
            {
                if (rightfunc == functype::not_a_function) fprintf (file, "%d [shape = record, style = filled, fillcolor = lightgray, color = black, label = \" {<data> memory: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right, ((part*) element -> right -> data) -> identificator);
                if (rightfunc == functype::declaration) fprintf (file, "%d [shape = record, style = filled, fillcolor = orange, color = black, label = \" {<data> function: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right, ((part*) element -> right -> data) -> identificator);
                if (rightfunc == functype::call) fprintf (file, "%d [shape = record, style = filled, fillcolor = yellow, color = black, label = \" {<data> func_call: %d | {<left> LEFT | <right> RIGHT}}\"]\n", element -> right, ((part*) element -> right -> data) -> identificator);
                break;
            }
            default: fprintf (file, "%d [shape = record, style = filled, fillcolor = red, color = white, label = \" {<data> UNCKNOWN (wtf) | {<left> LEFT | <right> RIGHT}}\"]\n", element); break;;
        }
        fprintf (file, "%d:<right> -> %d [color = black]\n", element, element -> right);
        dump (element -> right, file);
    }

    return 0;
}




//end
//zoluppa
