#include <fstream>
#include <cstring>
#include <cstdlib>
#include <assert.h>


#define DEBUG printf (">Debug output from %d line\n", __LINE__);
#define meow
#define isjump (((*program) [IP - 1] == CMD_JMP)   ||                           \
                ((*program) [IP - 1] == CMD_JE)    ||                           \
                ((*program) [IP - 1] == CMD_JNE)   ||                           \
                ((*program) [IP - 1] == CMD_JA)    ||                           \
                ((*program) [IP - 1] == CMD_JAE)   ||                           \
                ((*program) [IP - 1] == CMD_JB)    ||                           \
                ((*program) [IP - 1] == CMD_JBE)   ||                           \
                ((*program) [IP - 1] == CMD_CALL)) && (last_readed == COMMAND)


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

char        initial_file_name [100]   = {};
char        final_file_name [50]     = {};
char*       program_buffer           = 0;
long        f_size                   = 0;
int         last_readed              = 0;
const char  initialization [10]      = "KV_1.0";


struct label
{
    char labelname [20] = {};
    int line = 0;
};

label* labels = 0;
int labels_count = 0;


const int COMMAND  = 1;
const int PARAMETR = 2;
const int VAR_CMD  = 3;
const int VAR      = 4;



int         fake_program        (char** program_buffer);
long        file_size           (FILE* file);
long        read_program        (char** program_buffer);
void        write_program       (double** program, long pr_size);
long        prepare_labels      (char** program_buffer, long f_size);
int         addid               (char instruction [], int line);
int         getid               (char instruction []);
long        old_compile_program (char** program_buffer, double** program, long f_size);
long        compile_program     (char** program_buffer, double** program, long f_size);


/*!
«аводить названи€ переменных в оперативной пам€ти!
*/

int main (int argc, const char* argv [])
{
    if (argc >= 2) strcpy (initial_file_name, argv [1]);
    else
    {
        printf ("Enter name of file with program: ");
        scanf ("%s", &initial_file_name);
    }

    printf ("Enter name of file where to write: ");
    scanf ("%s", &final_file_name);

    f_size = read_program (&program_buffer);

    if (fake_program (&program_buffer))
    {
        printf ("Program is for another compiler!\n");
        return 1;
    }

    double* program = 0;
    program = (double*) calloc (f_size, sizeof(double));

    long pr_size = 0;

    prepare_labels (&program_buffer, f_size);
    printf ("%d\n", f_size);
DEBUG
    pr_size = compile_program (&program_buffer, &program, f_size);
    DEBUG
    pr_size = compile_program (&program_buffer, &program, f_size);
DEBUG
    write_program (&program, pr_size);

    return 0;
}



int fake_program (char** program_buffer)
{
    const int construction_length = 50;
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


long file_size (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long file_size = ftell (file);
    rewind (file);

    return file_size;
}


long read_program (char** program_buffer)
{
    printf ("read_program(\"%s\")\n", initial_file_name);
    FILE* file = fopen (initial_file_name, "r");
    assert (file);

    long f_size = file_size (file);

    *program_buffer = (char*) calloc (f_size, sizeof(char));
    f_size = fread (*program_buffer, sizeof(char), f_size, file);

    fclose (file);

    return f_size;
}



void write_program (double** program, long pr_size)
{
    FILE* file = fopen (final_file_name, "w");
    for (int counter = 0; counter < pr_size; counter++)
        fprintf (file, "%g ", (*program) [counter]);

    fclose (file);
}


long prepare_labels (char** program_buffer, long f_size)
{
    for (long counter = 0; counter < f_size; counter++)
    {
        if ((*program_buffer) [counter] == ':' && ((*program_buffer) [counter - 1] == '\n' || counter == 0))
            labels_count++;
    }

    labels = (label*) calloc (sizeof (label), labels_count);

    return 0;
}


int getid (char instruction [])
{
    int counter = 0;
    while (strcmp ((labels [counter]).labelname, instruction) && (labels [counter]).line != 0)
        counter++;

    return (labels [counter]).line;
}


int addid (char instruction [], int line)
{
    int counter = 0;
    while ((labels [counter]).line != 0)
        counter++;

    strcpy ((labels [counter]).labelname, instruction);
    (labels [counter]).line = line;

    return line;
}


long compile_program (char** program_buffer, double** program, long f_size)
{
    const int instruction_length = 20;
    char instruction [instruction_length] = {};
    int  instr_ind        = 0;
    long IP               = 0;
    int  is_commented     = 0;

    for (int counter = 0; counter < f_size; counter++)
    {
        if (((*program_buffer) [counter] == '#') || (is_commented))
        {
            is_commented = 1;
            if ((*program_buffer) [counter] == '\n') is_commented = 0;
        }

        else if (!isspace ((*program_buffer) [counter]))
            instruction [instr_ind++] = (*program_buffer) [counter];

        else
        {
            instruction [instr_ind] = '\0';
            printf ("current instruction %s, counter = %d\n", instruction, counter);
            if ((atoi (instruction)) || (*instruction == '0'))
            {
                (*program) [IP++] = atof (instruction);
                last_readed = PARAMETR;
            }

            else if (*instruction == ':')
            {
                if (isjump)
                    (*program) [IP++] = getid (instruction);

                else
                    addid (instruction, IP);

                last_readed = PARAMETR;
            }

            #define DEF_CMD( name, command, num, code )                 \
                    else if (!stricmp (instruction, #command))          \
                    {                                                   \
                        (*program) [IP++] = CMD_##name;                 \
                        last_readed = COMMAND;                          \
                    }

            #include "commands.h"

            #undef DEF_CMD


            else if (*instruction == '[')
            {
                (*program) [IP - 1] = (*program) [IP - 1] + 4;
                for (int inst_counter = 0; inst_counter <= instr_ind - 2; inst_counter++)
                    instruction [inst_counter] = instruction [inst_counter + 1];
                instruction [instr_ind - 2] = '\0';

                if ((atoi (instruction)) || (*instruction == 48))
                {
                    (*program) [IP++] = atof (instruction);
                    last_readed = PARAMETR;
                }

                else
                {
                    #define DEF_REG( name, num )                            \
                            if (!stricmp (instruction, #name))              \
                            {                                               \
                                (*program) [IP--] = REG_##name;             \
                                (*program) [IP] = (*program) [IP] + 2;      \
                                IP = IP + 2;                                \
                                last_readed = PARAMETR;                     \
                            }

                    #include "registers.h"

                    #undef DEF_REG
                }
            }

            else
            {
                #define DEF_REG( name, num )                            \
                        if (!stricmp (instruction, #name))              \
                        {                                               \
                            (*program) [IP--] = REG_##name;             \
                            (*program) [IP] = (*program) [IP] + 2;      \
                            IP = IP + 2;                                \
                            last_readed = PARAMETR;                     \
                        }

                #include "registers.h"

                #undef DEF_REG
            }

            instr_ind = 0;
        }
    }

    return IP;
}

///dsaf
