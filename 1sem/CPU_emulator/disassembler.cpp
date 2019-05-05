#include <fstream>
#include <cstring>
#include <cstdlib>
#include <assert.h>


#define DEBUG if(0)
#define meow


#define DEF_REG( name, num )    \
    REG_##name = num,

enum registers
{
    #include <C:\Users\vskar\Desktop\MIPT\Programming\Core i11\registers.h>
    REG_ERR = -1
};

#undef DEF_REG


#define DEF_CMD( name, command, num, code ) \
    CMD_##name = num,

enum commands
{
    #include <C:\Users\vskar\Desktop\MIPT\Programming\Core i11\commands.h>
    CMD_MAX
};

#undef DEF_CMD

char        initial_file_name [20]   = {};
char        final_file_name [20]     = {};
int         labels [10]              = {};


long    file_size       (FILE* file);
long    read_program    (double** program);
long    rebuild_program (char** program_buffer, const long f_size, double** program);
long    compile_program (double** program, const long pr_size);


int main ()
{
    printf ("Enter name of file with code: ");
    scanf ("%s", &initial_file_name);
    printf ("Enter name of file where to write: ");
    scanf ("%s", &final_file_name);

    double*  program = 0;

    long pr_size = read_program (&program);

    compile_program (&program, pr_size);
    compile_program (&program, pr_size);

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
    FILE* file = fopen (initial_file_name, "r");

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

    const int cmd_len  = 20;
    char cmd [cmd_len] = {};
    int  cmd_ind       = 0;

    *program = (double*) calloc (f_size, sizeof(int));

    for (long counter = 0; counter < f_size; counter++)
    {
        if ((!isspace ((*program_buffer) [counter])) || (counter == f_size - 1))
            cmd [cmd_ind++] = (*program_buffer) [counter];

        else
        {
            cmd [cmd_ind] = '\0';
            (*program) [IP++] = atoi (cmd);
            cmd_ind = 0;
        }
    }

    cmd [cmd_ind] = '\0';
    (*program) [IP++] = atoi (cmd);
    cmd_ind = 0;

    return IP;
}


long compile_program (double** program, const long pr_size)
{
    FILE* final_file = fopen (final_file_name, "w");

    int last_cmd  = 0;
    int labels_ind = 0;

    for (int counter = 0; counter < pr_size; counter++)
    {
        if (last_cmd == CMD_PUSH)
        {
            fprintf (final_file, "%g", (*program) [counter]);
            last_cmd = 0;
        }

        else if ((last_cmd == CMD_PUSHR) || (last_cmd == CMD_POPR))
        {
            #define DEF_REG( name, num )                            \
                    case num: fprintf (final_file, #name); break;

            switch ((int) ((*program) [counter]))
            {
                #include <C:\Users\vskar\Desktop\MIPT\Programming\Core i11\registers.h>
                default: fprintf (final_file, "undeclared_register"); break;
            }

            #undef DEF_REG

            last_cmd = 0;
        }

        else if ((last_cmd >= CMD_JMP) && (last_cmd <= CMD_CALL))
        {
            int num_of_label = 0;
            int label_exist  = 0;

            for (int l_counter = 0; l_counter < 10; l_counter++)
            {
                if (labels [l_counter] == (*program) [counter])
                {
                    num_of_label = l_counter;
                    label_exist  = 1;
                }
            }

            if (label_exist)
                fprintf (final_file, ":LABEL_%d", num_of_label);
            else
                labels [labels_ind++] = (*program) [counter];

            last_cmd = 0;
        }

        else if (last_cmd == CMD_PUSHM || last_cmd == CMD_POPM)
        {
            fprintf (final_file, "[%d]", (*program) [counter]);
            last_cmd = 0;
        }

        else if (last_cmd == CMD_PUSHRM || last_cmd == CMD_POPRM)
        {
            #define DEF_REG( name, num )                            \
                    case num:                                       \
                        {                                           \
                            fprintf (final_file, "[");              \
                            fprintf (final_file, #name);            \
                            fprintf (final_file, "]");              \
                            break;                                  \
                        }

            switch ((int) ((*program) [counter]))
            {
                #include <C:\Users\vskar\Desktop\MIPT\Programming\Core i11\registers.h>
                default: fprintf (final_file, "undeclared_register"); break;
            }

            #undef DEF_REG

            last_cmd = 0;
        }

        else
        {
            #define DEF_CMD( name, command, num, code )                                        \
                    case num: fprintf (final_file, #command); last_cmd = CMD_##name; break;

            switch ((int) ((*program) [counter]))
            {
                #include <C:\Users\vskar\Desktop\MIPT\Programming\Core i11\commands.h>
                default: fprintf (final_file, "undeclared_command");
            }

            #undef DEF_CMD
        }

        int is_label_here = 0;
        int num_of_label  = 0;

        for (int l_counter = 0; l_counter < 10; l_counter++)
            if (labels [l_counter] == counter + 1)
            {
                is_label_here = 1;
                num_of_label  = l_counter;
            }

        if (is_label_here) fprintf (final_file, "\n:LABEL_%d", num_of_label);


        if ((last_cmd != CMD_PUSH) && ((last_cmd < CMD_PUSHR) || (last_cmd > CMD_POPRM)) && ((last_cmd < CMD_JMP) || (last_cmd > CMD_CALL)))
            fprintf (final_file, "\n");
        else
            fprintf (final_file, " ");
    }
}
