/*!
    \file main.cpp
    \mainpage
    \brief ���������� �������

    ��������� ��������� ������� ������� �� �������� �� ������� � ������ �����
 */



#include <fstream>
#include <cstring>
#include <locale>
#include <cstdlib>
#include <assert.h>



const int console_mode = 0; const int file_mode = 1;



struct mystr
{
    char* str;
    int len;
};



int    mode_choose            ();
void   read_file              (mystr** lines, long* lines_count_adress, char** buffer);
long   text_preparing         (char* buffer, mystr lines[], const long text_size);
long   lines_counter          (char* buffer, const long text_size);
int    qcompare               (const void* line1, const void* line2);
int    reverse_qcompare       (const void* line1, const void* line2);
void   console_print          (const mystr lines[], const int lines_count);
void   file_print             (const mystr lines[], const int lines_count);
void   reverse_console_print  (const mystr lines[], const int lines_count);
void   reverse_file_print     (const mystr lines[], const int lines_count);




int    main                   ()
{
    setlocale(LC_ALL, "Russian");

    int mode = mode_choose();

    char* buffer      = 0;
    mystr* lines      = 0;
    long lines_count  = 0;

    read_file (&lines, &lines_count, &buffer);

    qsort (lines, lines_count, sizeof(mystr), qcompare);

    switch (mode)
    {
        case console_mode:  console_print(lines, lines_count); break;
        case file_mode:     file_print(lines, lines_count); break;
        default:            printf("Error in printing sorted file (Code: %d)\n", mode); break;
    }


    qsort(lines, lines_count, sizeof(mystr), reverse_qcompare);

    switch (mode)
    {
        case console_mode:  reverse_console_print(lines, lines_count); break;
        case file_mode:     reverse_file_print(lines, lines_count); break;
        default:            printf("Error in printing reverse sorted file (Code: %d)\n", mode); break;
    }
    return 0;
}



/*!
    ����� ������ ������ ���������
    \param [out] mode ����� ������
*/
int    mode_choose            ()
{
    int mode = 0;
    printf("0 - ���������� �����\n1 - �������� �����\n>>");
    scanf("%d", &mode);
    return mode;
}


/*!
    ������ ����� � �������� ������ � ������� � ����������� �� ������

    \param [in] lines ������ �� ������ � �������� �����
    \param [in] lines_count ������ �� long � ����������� �����
    \param [in] buffer ������ �� �����
*/
void   read_file              (mystr** lines, long* lines_count, char** buffer)
{
    FILE* text_link = fopen ("onegin.txt", "rb");
    fseek (text_link, 0, SEEK_END);
    long text_size = ftell (text_link);
    rewind (text_link);

    *buffer = (char*) calloc (text_size + 1, sizeof(char));

    fread(*buffer, sizeof(char), text_size, text_link);

    *lines = (mystr*) calloc (lines_counter(*buffer, text_size) + 1, sizeof(mystr));

    *lines_count = text_preparing (*buffer, *lines, text_size);
}


/*!
    ���������� ������ � ���������� ������� �� ��������
    \param [in] buffer ������ �� ������ � �������
    \param [in] lines ������ � �������� �����
    \param [in] text_size ������ ������
    \param [out] line ���������� ����� � ������
*/
long   text_preparing         (char* buffer, mystr lines[], const long text_size)
{
    assert(buffer != NULL);
    assert(lines != NULL);
    assert(text_size != NULL);

    long line       = 0;
    lines[line].str = buffer;
    line++;

    for (int counter = 1; counter < text_size; counter++)
    {
        if (buffer[counter] =='\n' )
        {
            buffer[counter]    = '\0';
            lines[line].str    = buffer + counter + 1;
            lines[line-1].len  = int(lines[line].str - lines[line - 1].str);
            line++;
        }
    }
    return line;
}


/*!
    ������� ���������� ����� ��� ��������� ������
    \param [in] buffer ����� � �������
    \param [in] text_size ������ ������
    \param [out] lines_count ���������� �����
*/
long   lines_counter          (char* buffer, const long text_size)
{
    assert(buffer != NULL);
    assert(text_size != NULL);

    long lines_count = 1;
    for (char* link = buffer; link < buffer + text_size; link++)
    {
        if (*link == '\n') lines_count++;
    }
    return lines_count;
}


/*!
    ���������� ��� ��������� ���� �����
    \param [in] line1 ������ ������
    \param [in] line2 ������ ������
*/
int    qcompare               (const void* line1, const void* line2)
{
    mystr s1 = *(mystr*)line1;
    mystr s2 = *(mystr*)line2;
    while (ispunct(*s1.str) || isspace(*s1.str)) s1.str++;
    while (ispunct(*s2.str) || isspace(*s2.str)) s2.str++;
    return strcmp(s1.str, s2.str);
}


/*!
    ���������� ��� ��������� ���� ����� (��������� � ����� ������)
    \param [in] line1 ������ ������
    \param [in] line2 ������ ������
*/
int    reverse_qcompare       (const void* line1, const void* line2)
{
    mystr s1          = *(mystr*)line1;
    mystr s2          = *(mystr*)line2;
    char* position1   = s1.str + s1.len - 2;
    char* position2   = s2.str + s2.len - 2;

    while (ispunct(*position1) || isspace(*position1)) position1--;
    while (ispunct(*position2) || isspace(*position2)) position2--;

    int counter = 0;

    while ((position1 - counter >= s1.str) && (position2 - counter >= s2.str))
    {
        int result    = *(position1 - counter) - *(position2 - counter);
        if (result != 0) return result;
        counter++;
    }
    return 0;
}


/*!
    ����� ������ � �������
    \param [in] lines ������ � �������� �����
    \param [in] lines_count ���������� �����
*/
void   console_print          (const mystr lines[], const int lines_count)
{
    assert(lines != NULL);
    assert(lines_count != NULL);

    for (int counter = 0; counter < lines_count; counter++)
    printf("%s\n", lines[counter].str);
}


/*!
    ����� ������ � ����
    \param [in] lines ������ � �������� �����
    \param [in] lines_count ���������� �����
*/
void   file_print             (const mystr lines[], const int lines_count)
{
    assert(lines != NULL);
    assert(lines_count != NULL);

    FILE* destination = fopen("onegin_s.txt", "wb");
    for (int counter = 0; counter < lines_count; counter++)
    {
        fprintf(destination, "%s\n", lines[counter].str);
    }
    fclose(destination);
}


/*!
    ����� ������ � ������� (��� ���������� � ������ �����)
    \param [in] lines ������ � �������� �����
    \param [in] lines_count ���������� �����
*/
void   reverse_console_print  (const mystr lines[], const int lines_count)
{
    assert(lines != NULL);
    assert(lines_count != NULL);

    for (int counter = 0; counter < lines_count; counter++)
    printf("%100s\n", lines[counter].str);
}


/*!
    ����� ������ � ���� (��� ���������� � ������ �����)
    \param [in] lines ������ � �������� �����
    \param [in] lines_count ���������� �����
*/
void   reverse_file_print     (const mystr lines[], const int lines_count)
{
    assert(lines != NULL);
    assert(lines_count != NULL);

    FILE* destination = fopen("onegin_rs.txt", "wb");
    for (int counter = 0; counter < lines_count; counter++)
    {
        fprintf(destination, "%s\n", lines[counter].str);
    }
    fclose(destination);
}
