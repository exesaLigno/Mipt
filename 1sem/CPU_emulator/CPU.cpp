/*!
    \file main.cpp
    \mainpage
    \brief Процессор
    \note Эмулятор процессора на стэке
*/



#include "stack_lib.cpp"
#include <fstream>
#include <locale>
#include <cmath>
#include <cstring>
#include <cstdlib>


#define DEBUG printf (">Debug output for %d line\n", __LINE__);



#define DEF_REG( name, num )    \
    REG_##name = num,

enum registers
{
    #include "registers.h"
    REG_ERR = -1
};

#undef DEF_REG


#define DEF_CMD( name, command, num, code ) \
    CMD_##name = num,

enum commands
{
    #include "commands.h"
    CMD_MAX
};

#undef DEF_CMD


const int  proc_stack_size      = 32;
const int  registers_count      = 10;
const int  call_stack_size      = 32;
const int  RAM_size             = 256;
const char initialization [10]  = "KV_1.0";
const int  name_length          = 20;


struct CPU
{
    stack_t proc_stack;
    stack_t call_stack;
    elem_t* registers;
    elem_t* RAM;
};


namespace global
{
    char program_name [name_length] = {};
}



int     print_call_stack(CPU* working_core, FILE* where_to_write);
int     parameterized   (int command);
int     fake_program    (char** program_buffer);
int     initiate_cpu    (CPU* processor);
long    file_size       (FILE* file);
long    read_program    (double** program_buffer);
long    rebuild_program (char** program_buffer, const long f_size, double** program);



int main (int argc, const char* argv [])
{
    FILE* log_file = fopen ("execution_log.txt", "w");
    if (argc >= 2) strcpy (global::program_name, argv [1]);
    else
    {
        printf ("Enter name of file with program: ");
        scanf ("%s", &global::program_name);
    }


    double* program = 0;

    CPU core1;
    initiate_cpu (&core1);

    long pr_size = read_program (&program);

    int IP = 0;

    while (program [IP] != CMD_END)
    {
        fprintf (log_file, "position %d >", IP);

        #define DEF_CMD( name, command, num, code )         \
                case CMD_##name: fprintf (log_file, "now executed command <%s> ", #name); break;

        switch ((int) program [IP])
        {
            #include "commands.h"
        }

        #undef DEF_CMD


        if (parameterized ((int) program [IP]))
            fprintf (log_file, "with parameter %lg", program [IP + 1]);

        if ((int) program [IP] == CMD_RET || (int) program [IP] == CMD_CALL)
            print_call_stack (&core1, log_file);

        fprintf (log_file, "\n");

        #define DEF_CMD( name, command, num, code )         \
                case CMD_##name:                            \
                {                                           \
                    code                                    \
                    break;                                  \
                }

        switch ((int) program [IP++])
        {
            #include "commands.h"
        }

        #undef DEF_CMD
    }

    printf ("\n\nexecuted\n----------------------------------------\n");

    system ("pause");

    return 0;
}



int fake_program (char** program_buffer)
{
    const int construction_length = 10;
    char construction [construction_length] = {};
    int ind = 0;
    while ((*program_buffer) [ind] != '\n')
    {
        construction [ind] = (*program_buffer) [ind];
        ind++;
    }
    construction [ind] = '\0';
    if (strcmp (construction, initialization)) return 1;
    else return 0;
}


int initiate_cpu (CPU* processor)
{
    stack_construct (&processor -> proc_stack, proc_stack_size);
    stack_construct (&processor -> call_stack, call_stack_size);
    processor -> registers = (elem_t*) calloc (registers_count, sizeof (elem_t));
    processor -> RAM = (elem_t*) calloc (RAM_size, sizeof (elem_t));
    return 0;
}


long file_size (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long file_size = ftell (file);
    rewind (file);

    return file_size;
}


long read_program (double** program)
{
    FILE* file = fopen (global::program_name, "r");

    long f_size = file_size (file);

    char* program_buffer = (char*) calloc (f_size, sizeof(char));
    fread (program_buffer, sizeof(char), f_size, file);

    fclose (file);

    long pr_size = rebuild_program (&program_buffer, f_size, program);

    return pr_size;
}


long rebuild_program (char** program_buffer, const long f_size, double** program)
{
    int IP = 0;

    const int  cmd_len  = 20;
    char cmd [cmd_len] = {};
    int  cmd_ind  = 0;

    *program = (double*) calloc (f_size, sizeof(double));

    for (long counter = 0; counter < f_size; counter++)
    {
        if ((!isspace ((*program_buffer) [counter])) || (counter == f_size - 1))
            cmd [cmd_ind++] = (*program_buffer) [counter];

        else
        {
            cmd [cmd_ind] = '\0';
            (*program) [IP++] = atof (cmd);
            cmd_ind = 0;
        }
    }

    cmd [cmd_ind] = '\0';
    (*program) [IP++] = atof (cmd);
    cmd_ind = 0;

    return IP;
}


int parameterized (int command)
{
    if (command == 1 || (command >= 3 && command <= 8) || (command >= 15 && command <= 22))
        return 1;

    return 0;
}


int print_call_stack (CPU* working_core, FILE* where_to_write)
{
    fprintf (where_to_write, "\n");

    for (int counter = 0; counter <= (working_core -> call_stack).pointer; counter++)
        fprintf (where_to_write, "%lg ", (working_core -> call_stack).buffer [counter]);

    fprintf (where_to_write, "\n");
}




/// На первое время можно сделать два дабловских и забивать в стэке вызовов на дробную часть
/*!
    Другое решение:
    Шаблонные классы/структуры
    Stack <double> theStack     = {};
    Stack <int>    retAddrStack = {};

    template <typename elemT>
    struct Stack
    {
        elemT data [100];
        ...
    }

    Шо делать с функциями

    template <typename elemT>
    StackPush (Stack <elemT>* stk, elemT wtp)
    {
        ...
        data[count++] = wtp;
        ...
    }

    +nandtotetris.org
    +Харрис и харрис плис fpga верилог
*/

