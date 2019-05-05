#define eltype      ((part*) (program_text_analized) [*pointer] -> data) -> type
#define elop        ((part*) (program_text_analized) [*pointer] -> data) -> op
#define elnumber    ((part*) (program_text_analized) [*pointer] -> data) -> number
#define elidentificator  ((part*) (program_text_analized) [*pointer] -> data) -> identificator
#define elfunc      ((part*) (program_text_analized) [*pointer] -> data) -> is_func

#define nexttype      ((part*) (program_text_analized) [*pointer + 1] -> data) -> type
#define nextop        ((part*) (program_text_analized) [*pointer + 1] -> data) -> op
#define nextnumber    ((part*) (program_text_analized) [*pointer + 1] -> data) -> number
#define nextvariable  ((part*) (program_text_analized) [*pointer + 1] -> data) -> variable
#define nextfunc      ((part*) (program_text_analized) [*pointer + 1] -> data) -> is_func


#define elemtype      ((part*) element -> data) -> type
#define elemop        ((part*) element -> data) -> op
#define elemnumber    ((part*) element -> data) -> number
#define elemidentificator  ((part*) element -> data) -> identificator
#define elemfunc      ((part*) element -> data) -> is_func

#define lefttype      ((part*) element -> left -> data) -> type
#define leftop        ((part*) element -> left -> data) -> op
#define leftnumber    ((part*) element -> left -> data) -> number
#define leftidentificator  ((part*) element -> left -> data) -> identificator
#define leftfunc      ((part*) element -> left -> data) -> is_func

#define righttype      ((part*) element -> right -> data) -> type
#define rightop        ((part*) element -> right -> data) -> op
#define rightnumber    ((part*) element -> right -> data) -> number
#define rightidentificator  ((part*) element -> right -> data) -> identificator
#define rightfunc      ((part*) element -> right -> data) -> is_func


#define isnumber( elem )    ((elem >= '0' && elem <= '9') || elem == '.')
#define isletter( elem )    ((elem >= 'a' && elem <= 'z') || (elem >= 'A' && elem <= 'Z') || elem == '_')
#define issym( elem )       ((elem >= '!' && elem <= '/') || (elem >= ':' && elem <= '?') || (elem >= '[' && elem <= '^') || (elem >= '{' && elem <= '~'))
#define isbracket( elem )   (elem == '(' || elem == ')' || elem == '[' || elem == ']' || elem == '{' || elem == '}')


#define _GetG0      grammatic::GetG0        (program_tree, program_text_analized, pointer)

#define _GetDef     grammatic::GetDef       (program_tree, program_text_analized, pointer)
#define _GetOp      grammatic::GetOp        (program_tree, program_text_analized, pointer)

#define _GetStdFunc grammatic::GetStdFunc   (program_tree, program_text_analized, pointer)
#define _GetAssgn   grammatic::GetAssgn     (program_tree, program_text_analized, pointer)
#define _GetWhile   grammatic::GetWhile     (program_tree, program_text_analized, pointer)
#define _GetFor     grammatic::GetFor       (program_tree, program_text_analized, pointer)
#define _GetIf      grammatic::GetIf        (program_tree, program_text_analized, pointer)
#define _GetElse    grammatic::GetElse      (program_tree, program_text_analized, pointer)
#define _GetCall    grammatic::GetCall      (program_tree, program_text_analized, pointer)
#define _GetRet     grammatic::GetRet       (program_tree, program_text_analized, pointer)

#define _GetItemize grammatic::GetItemize   (program_tree, program_text_analized, pointer)
#define _GetLength  grammatic::GetLength    (program_tree, program_text_analized, pointer)

#define _GetC       grammatic::GetC         (program_tree, program_text_analized, pointer)
#define _GetE       grammatic::GetE         (program_tree, program_text_analized, pointer)
#define _GetT       grammatic::GetT         (program_tree, program_text_analized, pointer)
#define _GetF       grammatic::GetF         (program_tree, program_text_analized, pointer)
#define _GetP       grammatic::GetP         (program_tree, program_text_analized, pointer)

#define _GetN       grammatic::GetN         (program_tree, program_text_analized, pointer)
#define _GetId      grammatic::GetId        (program_tree, program_text_analized, pointer)



#define iscompare( elem ) \
        (elem == operator_more || elem == operator_moreeq || elem == operator_less || elem == operator_lesseq || elem == operator_equal)


#define next( link ) \
        printf ("pointer on %d line changed from %d to %d\n", __LINE__, *pointer, *pointer + 1);  \
        (*link)++;


node* operator+ (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_plus;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator- (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_minus;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator* (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_mul;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator/ (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_div;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator^ (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_power;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator> (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_more;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator>= (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_moreeq;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator< (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_less;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* operator<= (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_lesseq;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}



node* operator== (node &left_element, node &right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_equal;

    node* new_node = create_node (op);
    new_node -> left = &left_element;
    new_node -> right = &right_element;

    return new_node;
}


node* add_assgn (node* left_element, node* right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_assgn;

    node* new_node = create_node (op);
    new_node -> left = left_element;
    new_node -> right = right_element;

    return new_node;
}


node* add_op (node* left_element, node* right_element)
{
    part op;
    op.type = type::op_node;

    node* new_node = create_node (op);
    new_node -> left = left_element;
    new_node -> right = right_element;

    return new_node;
}


node* add_def (node* left_element, node* right_element)
{
    part op;
    op.type = type::def_node;

    node* new_node = create_node (op);
    new_node -> left = left_element;
    new_node -> right = right_element;

    return new_node;
}


node* create_def (node* def, node* left_element, node* right_element)
{
    def -> left = left_element;
    def -> right = right_element;

    return def;
}


node* add_if (node* left_element, node* right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_if;

    node* new_node = create_node (op);
    new_node -> left = left_element;
    new_node -> right = right_element;

    return new_node;
}


node* add_while (node* left_element, node* right_element)
{
    part op;
    op.type = type::op;
    op.op = operator_while;

    node* new_node = create_node (op);
    new_node -> left = left_element;
    new_node -> right = right_element;

    return new_node;
}







//end
