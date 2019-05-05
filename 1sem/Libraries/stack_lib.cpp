/*!
    \file stack_lib.cpp
    \mainpage
    \brief ����
    \note ���������� �������� �������� ����� � �������, ����������� ��� ��� ������
*/

#include <fstream>
#include <cstdlib>
#include <assert.h>

#define DEBUG if(0)

#define DECLARE(type, name, len)  \
type##_t name = {#name};          \
type##_construct (&name, len);


#define ASSERT_OK(type, name)          \
if (!type##_ok (name))                 \
{                                      \
    type##_dump (name);                \
    assert (! "��� ������, ��������"); \
}



//! ��� ��������� ������
int        last_error                 = 0;

//!�������� ���-�������
long       hash                       = 0;

//! ���� ������
const int  OUT_OF_STACK_INDEX         = 1;
const int  UNAUTHORIZED_STACK_EDITION = 2;


typedef int elem_t;


struct    stack_t
{
    char*    name;
    elem_t*  buffer;
    long     pointer;
    long     len;
};


long      search_hash      (stack_t* stack);
int       stack_ok         (stack_t* stack);
int       stack_construct  (stack_t* stack, const long length);
int       stack_empty      (stack_t* stack);
long      stack_size       (stack_t* stack);
elem_t    push             (stack_t* stack, const elem_t element);
elem_t    pop              (stack_t* stack);
elem_t    top              (stack_t* stack);
void      stack_dump       (stack_t* stack);



/*!
    ���������� ��� �������
    \param [in]  stack      ������ �� ����
    \param [out] hash_local ���������� ���
*/
long      search_hash      (stack_t* stack)
{
    long hash_local = stack -> pointer + 2 * stack -> len;
    for (int counter = 0; counter < stack -> len; counter++)
    {
        hash_local += (counter + 3) * ((char*)(stack -> buffer))[counter];
        //printf ("������ ������ ������� %d %d\n", counter, (stack -> buffer)[counter]);
    }
    //printf ("������� ��� ����� %d\n", hash_local);
    return hash_local;
}


/*!
    �������� ����� �� ����
    \param [in]  stack       ������ �� ����
    \param [out] stack_is_ok ������������ �����
*/
int       stack_ok         (stack_t* stack)
{
    int stack_is_ok = 1;

    if (hash != search_hash (stack))
    {
        stack_is_ok = 0;
        last_error  = UNAUTHORIZED_STACK_EDITION;
    }

    if ((stack -> pointer < 0) || (stack -> pointer > stack -> len))
    {
        stack_is_ok = 0;
        last_error  = OUT_OF_STACK_INDEX;
    }

    return stack_is_ok;
}


/*!
    ����������� �����
    \param [in] stack   ������ �� ����
    \param [in] length  ����� �����
    \param [in] type    ����� ���� ������, �������� � ����� (?)
*/
int       stack_construct  (stack_t* stack, const long length)
{
    stack -> pointer            = 0;
    stack -> len                = length;
    stack -> buffer             = (elem_t*) calloc (length, sizeof(elem_t));
    //stack -> buffer[0]          = 1337;
    //stack -> buffer[length + 1] = 1337;
    DEBUG hash = search_hash (stack);
    return 0;
}


/*!
    �������� ����� �� �������
    \param [in] stack ������ �� ����
*/
int       stack_empty      (stack_t* stack)
{
    DEBUG ASSERT_OK (stack, stack);
    int empty_attribute = 0;
    if (stack -> pointer == 0) empty_attribute = 1;
    return empty_attribute;
}


/*!
    ����������� �������� ������� �����
    \param [in] stack ������ �� ����
*/
long      stack_size       (stack_t* stack)
{
    DEBUG ASSERT_OK (stack, stack);
    return stack -> pointer;
}


/*!
    ���������� �������� � ����
    \param [in] stack    ������ �� ����
    \param [in] element  ����������� �������
*/
elem_t    push             (stack_t* stack, const elem_t element)
{
    DEBUG ASSERT_OK (stack, stack);
    stack -> buffer[stack -> pointer] = element;
    stack -> pointer++;
    DEBUG hash = search_hash (stack);
    DEBUG ASSERT_OK (stack, stack);
    return element;
}


/*!
    �������� �������� �� ����� (���������� ��������� �������)
    \param [in]  stack    ������ �� ����
    \param [out] element  ��������� �������
*/
elem_t    pop              (stack_t* stack)
{
    DEBUG ASSERT_OK (stack, stack);
    stack -> pointer--;
    elem_t element = stack -> buffer[stack -> pointer];
    DEBUG hash = search_hash (stack);
    DEBUG ASSERT_OK (stack, stack);
    return element;
}


/*!
    �������� ��������� �������� �����
    \param [in]  stack    ������ �� ����
    \param [out] element  ��������� �������

*/
elem_t    top              (stack_t* stack)
{
    DEBUG ASSERT_OK (stack, stack);
    stack -> pointer--;
    elem_t element = stack -> buffer[stack -> pointer];
    stack -> pointer++;
    DEBUG hash = search_hash (stack);
    DEBUG ASSERT_OK (stack, stack);
    return element;
}


/*!
    ������ ����� ������ � ���� ����
    \param [in] stack ������ �� ����
*/
void      stack_dump       (stack_t* stack)
{
    FILE* log_file = fopen ("log.txt", "w");

    char* error_name = "?";

    #define GET_DESCR_(code)  case code: error_name = #code; break;

    switch (last_error)
    {
        GET_DESCR_ (OUT_OF_STACK_INDEX);
        GET_DESCR_ (UNAUTHORIZED_STACK_EDITION);
    }

    #undef GET_DESCR_

    fprintf (log_file, "ERROR %d %s IN STACK <%s>[%p]\n", last_error, error_name, stack -> name, stack);
    fprintf (log_file, "  Length = %d\n", stack -> len);
    fprintf (log_file, "  Pointer position = %d\n", stack -> pointer);
    for (int counter = 0; counter < stack -> len; counter++)
        fprintf (log_file, "    %s[%d] = %d\n", stack -> name, counter, (stack -> buffer)[counter]);
    fclose (log_file);
}
