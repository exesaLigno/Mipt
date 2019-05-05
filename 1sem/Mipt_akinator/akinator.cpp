#include <fstream>
#include "tree_lib.cpp"
#include "stack_lib.cpp"
#include <locale>
#include "TXLib.h"
#include <windows.h>


#define send_to(code)                                       \
        FILE* linking = fopen (global::linking_file, "w");  \
        fprintf code;                                       \
        fclose (linking);                                   \

#define read_from(code)                                     \
        FILE* linking = fopen (global::linking_file, "r");  \



#define DEBUG printf ("{DEBUG OUTPUT FROM %d LINE}\n", __LINE__);


int     read_database           (tree_t* database);
int     read_database           (tree_t* database, char** buffer, int* counter, long buffer_size, node* last_added, int side);
int     write_database          (tree_t* database);
int     write_database          (node* active_element, int tabulation, FILE* file);
int     suppose                 (node* element);
node*   database_construct      (tree_t* database);

int     read_new                ();
int     add_new                 (tree_t* database, node* active_element);

int     give_characteristic     (tree_t* database, node* active_element);
int     compare_characteristic  (tree_t* database, node* element1, node* element2);


namespace global
{
    const int string_length = 50;
    const char filename [20] = "database.txt";
    char linking_file [20] = "non_user";
}



int main (int argc, const char* argv [])
{
    if (argc >= 2) strcpy (global::linking_file, argv [1]);

    int EXIT = 0;

    setlocale (LC_CTYPE, "Russian");
    tree_t database;
    //database_construct (&database);
    read_database (&database);

    int answer = 0;
    node* last_guessed = 0;

    while (!EXIT)
    {

        node* active_element = database.root;

        while (active_element -> left != 0 && active_element -> right != 0)
        {
            //send_to ((linking, "ak: Тот, кого вы загадали, %s?", active_element -> data));
            printf ("Тот, кого вы загадали, %s? (1/0): ", active_element -> data);
            answer = suppose (active_element);
            //scanf ("%d", &answer);
            if (answer)
                active_element = active_element -> right;
            else
                active_element = active_element -> left;
        }

        send_to ((linking, "Я полагаю, что тот, кого вы загадали - %s.", active_element -> data));
        printf ("Я полагаю, что тот, кого вы загадали - %s. Я прав? (1/0): ", active_element -> data);
        scanf ("%d", &answer);

        if (answer)
        {
            send_to ((linking, "Я угадал!\n"))
            printf ("Я угадал!\n");
            give_characteristic (&database, active_element);
            if (last_guessed && last_guessed != active_element)
            {
                send_to ((linking, "Выяснить, чем %s от %s?", last_guessed -> data, active_element -> data))
                printf ("Выяснить, чем %s от %s? (1/0): ", last_guessed -> data, active_element -> data);
                scanf ("%d", &answer);
                if (answer)
                    compare_characteristic (&database, last_guessed, active_element);
            }

            last_guessed = active_element;
        }

        else
            add_new (&database, active_element);

        printf ("Выйти? (1/0): ");

        scanf ("%d", &EXIT);
    }

    write_database (&database);

    service::dot_dump (&database);

    return 0;
}



node* database_construct (tree_t* database)
{
    node* last = tree_lpush (database, 0, "животное");
    tree_rpush (database, last, "кот");
    last = tree_lpush (database, last, "препод");
    tree_lpush (database, last, "Путин");
    last = tree_rpush (database, last, "ведет матан");
    tree_rpush (database, last, "Голубов");
    tree_lpush (database, last, "Бек");
    return 0;
}

 #define NOT_A_BOSS_JOB(verb,func)

int add_new (tree_t* database, node* active_element)
{
    char reading_buffer [global::string_length] = {};
    printf ("Похоже, я не знаю, кого вы загадали\nПожалуйста, напишите, кого вы загадали: ");

    fflush (stdin);
    gets (reading_buffer);

    tree_rpush (database, active_element, reading_buffer);
    tree_lpush (database, active_element, active_element -> data);

    printf ("Введите, чем %s отличается от %s: ", reading_buffer, active_element -> data);

    char reading_buffer2 [global::string_length] = {};
    fflush (stdin);
    gets (reading_buffer2);

    NOT_A_BOSS_JOB(using, strcpy)
        {
        int counter = 0;
        while (reading_buffer2 [counter] != '\0')
            active_element -> data [counter] = reading_buffer2 [counter++];
        active_element -> data [counter] = reading_buffer2 [counter];
        }

    return 0;
}


int give_characteristic (tree_t* database, node* active_element)
{
    const int stack_length = 100;
    DECLARE (stack, way, stack_length);
    printf ("Тот, кого вы загадали ");
    while (active_element -> prev)
    {
        if (active_element == active_element -> prev -> right)
            push (&way, 1);

        else if (active_element == active_element -> prev -> left)
            push (&way, 0);

        else
            push (&way, -1);

        active_element = active_element -> prev;
    }


    while (active_element -> left && active_element -> right)
    {
        int flag = pop (&way);

        if (flag)
        {
            printf ("%s", active_element -> data);
            active_element = active_element -> right;
            if (active_element -> left && active_element -> right)
                printf (", ");
            else
                printf (".\n");
        }

        else
        {
            printf ("не %s", active_element -> data);
            active_element = active_element -> left;
            if (active_element -> left && active_element -> right)
                printf (", ");
            else
                printf (".\n");
        }
    }

    return 0;
}


int compare_characteristic (tree_t* database, node* element1, node* element2)
{
    const int stack_length = 100;
    DECLARE (stack, way1, stack_length);
    DECLARE (stack, way2, stack_length);

    node* active_element = database -> root;

    printf ("%s и %s различны тем, что первый ", element1 -> data, element2 -> data);

    while (element1 -> prev)
    {
        if (element1 == element1 -> prev -> right)
            push (&way1, 1);

        else if (element1 == element1 -> prev -> left)
            push (&way1, 0);

        else
            push (&way1, -1);

        element1 = element1 -> prev;
    }


    while (element2 -> prev)
    {
        if (element2 == element2 -> prev -> right)
            push (&way2, 1);

        else if (element2 == element2 -> prev -> left)
            push (&way2, 0);

        else
            push (&way2, -1);

        element2 = element2 -> prev;
    }

    while (top (&way1) == top (&way2))
    {
        int flag = pop (&way1);
        int test = pop (&way2);
        if (flag)
            active_element = active_element -> right;

        else
            active_element = active_element -> left;
    }

    while (active_element -> right && active_element -> left)
    {
        int flag = pop (&way1);

        if (flag)
        {
            printf ("%s", active_element -> data);
            active_element = active_element -> right;
            if (active_element -> left && active_element -> right)
                printf (", ");
            else
                printf (".\n");
        }

        else
        {
            printf ("не %s", active_element -> data);
            active_element = active_element -> left;
            if (active_element -> left && active_element -> right)
                printf (", ");
            else
                printf (".\n");
        }
    }
}


int read_file (char** buffer)
{
    FILE* file = fopen (global::filename, "rb");
    fseek (file, 0, SEEK_END);
    long file_size = ftell (file);
    rewind (file);

    *buffer = (char*) calloc (file_size + 1, sizeof(char));

    fread (*buffer, sizeof(char), file_size, file);

    return file_size;
}


int read_database (tree_t* database)
{
    char* buffer = 0;

    long buffer_size = read_file (&buffer);

    for (int counter = 0; counter < buffer_size; counter++)
    {
        if (buffer [counter] == '{' || buffer [counter] == '(')
            read_database (database, &buffer, &counter, buffer_size, 0, 1);
    }

    return 0;
}


int read_database (tree_t* database, char** buffer, int* counter, long buffer_size, node* last_added, int side)
{
    char str [global::string_length] = {};
    int local_counter = 0;
    *counter += 2;

    while ((*buffer) [*counter] != '\'')
        str [local_counter++] = (*buffer) [(*counter)++];


    str [local_counter] = '\0';

    node* last = 0;


    if (side) last = tree_rpush (database, last_added, str);
    else last = tree_lpush (database, last_added, str);

    int is_first = 0;

    while (*counter < buffer_size)
    {
        if (((*buffer) [*counter] == '{' || (*buffer) [*counter] == '(') && (!is_first))
        {
            read_database (database, buffer, counter, buffer_size, last, 1);
            is_first = 1;
        }

        if (((*buffer) [*counter] == '{' || (*buffer) [*counter] == '(') && (is_first))
            read_database (database, buffer, counter, buffer_size, last, 0);

        (*counter)++;

        if ((*buffer) [*counter] == '}' || (*buffer) [*counter] == ')')
                return 0;
    }

    return 0;
}


int write_database (tree_t* database)
{
    FILE* file = fopen (global::filename, "w");

    int tabulation = 0;
    node* active_element = database -> root;

    fprintf (file, "'\n{");
    write_database (active_element, tabulation, file);
    fprintf (file, "}\n");

    fclose (file);

    return 0;
}


int write_database (node* active_element, int tabulation, FILE* file)
{
    fprintf (file, "'%s'\n", active_element -> data);


    if (active_element -> left)
    {
        for (int counter = 0; counter < tabulation; counter++)
            fprintf(file, " ");
        fprintf (file, "(");

        write_database (active_element -> right, tabulation + 4, file);

        for (int counter = 0; counter < tabulation; counter++)
            fprintf(file, " ");
        fprintf (file, ")\n");
    }

    if (active_element -> right)
    {
        for (int counter = 0; counter < tabulation; counter++)
            fprintf(file, " ");
        fprintf (file, "(");

        write_database (active_element -> left, tabulation + 4, file);

        for (int counter = 0; counter < tabulation; counter++)
            fprintf(file, " ");
        fprintf (file, ")\n");
    }

    return 0;
}


int suppose (node* elelement)
{
    FILE* linking = fopen (linking_file, "w");
    fprintf ("Тот, кого вы загадали, %s?", element -> data)

}

//end
