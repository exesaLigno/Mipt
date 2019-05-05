#include <fstream>
#include <cstdlib>
#include <locale>

#define DEBUG
#define meow


#define lbuffer list -> buffer
#define ldata( k ) (*(list -> buffer + k)).data
#define lprev( k ) (*(list -> buffer + k)).prev
#define lnext( k ) (*(list -> buffer + k)).next
#define lord( k )  (*(list -> buffer + k)).ordinal_index
#define llength list -> length
#define lhead list -> head
#define ltail list -> tail
#define lfree list -> free
#define llast list -> last_inserted
#define elem1_isfirst (elem1 -> data == 'POISON') && (elem1 -> next == 0) && (elem1 -> prev == 0)
#define elem2_isfirst (elem2 -> data == 'POISON') && (elem2 -> next == 0) && (elem2 -> prev == 0)


struct hash_info
{
    int hash_sum;
    int count_of_sums;
};

typedef hash_info elem_t;


struct cell
{
    elem_t data;
    int next;
    int prev;
    int ordinal_index;
};

struct list_t
{
    cell*   buffer          = 0;
    int     length          = 0;
    int     head            = 0;
    int     tail            = 0;
    int     free            = 0;
    int     last_inserted   = 0;
};


namespace ERR
{
    const int PREV_NOT_EXIST = 1;
    const int NEXT_NOT_EXIST = 2;
    const int LIST_OVERFLOW  = 3;
}



int         list_constructor        (list_t* list, long length);
int         list_ainsert            (list_t* list, int position, elem_t element);
int         list_binsert            (list_t* list, int position, elem_t element);
int         list_push               (list_t* list, elem_t element);
elem_t      list_pop                (list_t* list);
elem_t      list_remove             (list_t* list, int position);


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
    FILE* log = fopen ("log.txt", "w");

    int     list_real_dump          (list_t* list);
    int     list_dump               (list_t* list);
    int     dot_dump                (list_t* list);
}


/*
int main ()
{
    setlocale (LC_CTYPE, "Russian");
    int lst_len = 6;
    list_t lst;
    list_constructor (&lst, lst_len);

    printf ("%d %d\n", list_ainsert (&lst, 0, 30), lst.free);
    printf ("%d %d\n", list_ainsert (&lst, 1, 50), lst.free);
    printf ("%d %d\n", list_ainsert (&lst, 2, 60), lst.free);
    printf ("%d %d\n", list_binsert (&lst, 2, 40), lst.free);
    printf ("%d %d\n", list_binsert (&lst, 1, 20), lst.free);
    printf ("%d %d\n", list_binsert (&lst, 5, 10), lst.free);
    //printf ("Ord to real %d\n", slow::list_index_ordtoreal (&lst, 3));
    //printf ("Real to ord %d\n", slow::list_index_realtoord (&lst, 4));
    //list_insert (&lst, 2, 45);
    //list_remove (&lst, 2);
    service::list_real_dump (&lst);
    service::list_dump (&lst);
    service::dot_dump (&lst);

    slow::list_sort (&lst);

    service::list_real_dump (&lst);
    service::list_dump (&lst);
    return 0;
}
*/


int list_constructor (list_t* list, long length)
{
    lbuffer = (cell*) calloc (length + 1, sizeof (cell));
    llength = length;
    lhead = 0;
    ltail = 0;
    lfree = 1;
    lprev (0) = 0;
    lprev (length) = -1;
    ldata (0) = 'POISON';
    ldata (length) = 'POISON';
    lord (0) = 0;
    for (int k = 1; k < length; k++)
    {
        ldata (k) = 'POISON';
        lnext (k) = k + 1;
        lprev (k) = -1;
        lord (k) = 0;
    }
    llast = 0;
    return 0;
}


int list_ainsert (list_t* list, int position, elem_t data)
{
    if ((ldata (position) == 'POISON') && (position != 0) ||
        (position == 0) && (lhead != 0))
        return ERR::PREV_NOT_EXIST;

    if (lfree == 0)
        return ERR::LIST_OVERFLOW;

    int free_position = lfree;
    lfree = lnext (lfree);

    ldata (free_position) = data;
    lnext (free_position) = lnext (position);
    lprev (free_position) = position;

    if (lnext (position) != 0)
        lprev (lnext (position)) = free_position;

    if (position != 0)
        lnext (position) = free_position;
    else
        lhead = free_position;

    if (position == ltail)
        ltail = free_position;

    llast = free_position;

    return 0;
}


int list_binsert (list_t* list, int position, elem_t data)
{
    if (ldata (position) == 'POISON')
        return ERR::NEXT_NOT_EXIST;

    if (lfree == 0)
        return ERR::LIST_OVERFLOW;

    int free_position = lfree;
    lfree = lnext (lfree);

    ldata (free_position) = data;
    lnext (free_position) = position;
    lprev (free_position) = lprev (position);

    if (lprev (position) != 0)
        lnext (lprev (position)) = free_position;
    else
        lhead = free_position;

    lprev (position) = free_position;

    llast = free_position;

    return 0;
}


int list_push (list_t* list, int element)
{
    int code = list_ainsert (list, ltail, element);
    return code;
}


elem_t list_pop (list_t* list)
{
    elem_t element = list_remove (list, ltail);
    return element;
}


elem_t list_remove (list_t* list, int position)
{
    elem_t data = ldata (position);
    ldata (position) = 'POISON';

    if (lprev (position))
        lnext (lprev (position)) = lnext (position);
    else
        lhead = lnext (position);

    if (lnext (position))
        lprev (lnext (position)) = lprev (position);
    else
        ltail = lprev (position);

    lnext (position) = lfree;
    lprev (position) = -1;
    lfree = position;

    return data;
}


int slow::list_index_ordtoreal (list_t* list, int ordinal_index)
{
    int element = lhead;
    for (int counter = 0; counter < ordinal_index; counter++)
        element = lnext (element);
    return element;
}


int slow::list_index_realtoord (list_t* list, int real_index)
{
    const int ELEM_NOT_EXIST = -1;

    int element = lhead;
    int logical_index = 0;
    int is_exist = 0;
    while ((element != real_index) && (element != 0))
    {
        element = lnext (element);
        logical_index++;
    }

    if (element == real_index)
        is_exist = 1;

    if (is_exist)
        return logical_index;
    else
        return ELEM_NOT_EXIST;
}


int slow::list_sort (list_t* list)
{
    for (int counter = 0; counter < llength + 1; counter++)
        lord (counter) = slow::list_index_realtoord (list, counter);

    qsort (lbuffer, llength + 1, sizeof (cell), slow::list_compare);

    lhead = 1;
    int counter = lhead;
    while ((lprev (counter) != -1) && (counter < llength + 1))
    {
        lnext (counter) = counter + 1;
        if ((lprev (counter + 1) == -1) || (counter == llength))
        {
            lnext (counter) = 0;
            ltail = counter;
        }
        counter++;
    }

    for (counter = ltail; counter > 0; counter--)
        lprev (counter) = counter - 1;

    return 0;
}


int slow::list_compare (const void* e1, const void* e2)
{
    cell* elem1 = (cell*) e1;
    cell* elem2 = (cell*) e2;

    if ((elem1 -> data == 'POISON') && (elem1 -> next == 0) && (elem1 -> prev == 0))
        return -1;

    else if ((elem2 -> data == 'POISON') && (elem2 -> next == 0) && (elem2 -> prev == 0))
        return 1;

    else if ((elem2 -> prev == -1) && (elem1 -> prev != -1))
        return -1;

    else if ((elem1 -> prev == -1) && (elem2 -> prev != -1))
        return 1;

    else
        return elem1 -> ordinal_index - elem2 -> ordinal_index;

    return 0;
}


int slow::list_defrag (list_t* list)
{
    slow::list_sort (list);
    return 0;
}


int slow::list_expand (list_t* list, int new_length)
{
    return 0;
}


int slow::list_squeeze (list_t* list, int new_length)
{
    return 0;
}


int service::list_real_dump (list_t* list)
{
    long list_length = llength;

    fprintf (service::log, "\n<<REAL_DUMP>>\n\nHEAD = %d\nTAIL = %d\nFREE = %d\nLAST_INSERTED = %d\n\n{\n",
             lhead, ltail, lfree, llast);

    fprintf (service::log, "  NUMBER       DATA NEXT PREV\n\n");
    for (int counter = 0; counter < list_length + 1; counter++)
        if (ldata (counter) == 'POISON')
            fprintf (service::log, "  No %3d     POISON %4d %4d\n",
                     counter,
                     lnext (counter), lprev (counter));
        else
            fprintf (service::log, "  No %3d %10d %4d %4d\n",
                     counter, ldata (counter),
                     lnext (counter), lprev (counter));

    fprintf (service::log, "}\n");
    return 0;
}


int service::list_dump (list_t* list)
{
    int element = lhead;
    int counter = 0;

    fprintf (service::log, "\n<<LIST_DUMP>>\n\n");

    while (element != 0)
    {
        fprintf (service::log, "No %3d %10d %3d\n", counter++, ldata (element), element);
        element = lnext (element);
    }
}


int service::dot_dump (list_t* list)
{
    int pointer = lhead;
    FILE* file = fopen ("graph_dump.txt", "w");
    fprintf (file, "digraph G{\n");
    fprintf (file, "rankdir = LR\n");
    fprintf (file, "info [shape = record, style = rounded, color = blue, label = \"Length = %d | <head> Head = %d | Tail = %d | <free> Free = %d | Last inserted = %d \"]", pointer, llength, lhead, ltail, lfree, llast);
    fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \"<num> Number: %d | <data> Data: %d | {<prev> Prev: %d | <next> Next: %d}\"]\n", pointer, pointer, ldata (pointer), lprev (pointer), lnext (pointer));
    fprintf (file, "info:<head> -> %d:<data>[color = grey]\n", pointer);

    do
    {
        pointer = lnext (pointer);
        fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \"<num> Number: %d | <data> Data: %d | {<prev> Prev: %d | <next> Next: %d}\"]\n", pointer, pointer, ldata (pointer), lprev (pointer), lnext (pointer));
        fprintf (file, "%d:<next> -> %d:<data>[color = blue]\n", lprev (pointer), pointer);
        fprintf (file, "%d:<prev> -> %d:<data>[color = grey]\n", pointer, lprev (pointer));
    }
    while (lnext (pointer));

    int free_pointer = lfree;
    int last_free = -1;
    while (last_free != 0)
    {
        if (last_free == -1) last_free = 0;
        if (free_pointer == 0) break;
        fprintf (file, "%d [shape = record, style = rounded, color = blue, label = \"<num> Number: %d | <data> POISON | {<prev> Prev: %d | <next> Next: %d}\"]\n", free_pointer, free_pointer, lprev (free_pointer), lnext (free_pointer));
        if (last_free)
            fprintf (file, "%d:<next> -> %d:<data>[color = blue]\n", last_free, free_pointer);
        else
            fprintf (file, "info:<free> -> %d:<data>[color = blue]\n", free_pointer);

        last_free = free_pointer;
        free_pointer = lnext (free_pointer);
    }

    fprintf (file, "}");
    fclose (file);
    system ("dot -Tpng graph_dump.txt -o dump.png");
    system ("del graph_dump.txt");
    system ("dump.png");

    return 0;
}

//end
