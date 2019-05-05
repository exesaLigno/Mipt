#define eltype      ((func_element*) element -> data) -> type
#define elop        ((func_element*) element -> data) -> op
#define elnumber    ((func_element*) element -> data) -> number
#define elconstanta ((func_element*) element -> data) -> constanta
#define elvariable  ((func_element*) element -> data) -> variable

#define newtype      ((func_element*) new_node -> data) -> type
#define newop        ((func_element*) new_node -> data) -> op
#define newnumber    ((func_element*) new_node -> data) -> number
#define newconstanta ((func_element*) new_node -> data) -> constanta
#define newvariable  ((func_element*) new_node -> data) -> variable

#define lefttype      ((func_element*) element -> left -> data) -> type
#define leftop        ((func_element*) element -> left -> data) -> op
#define leftnumber    ((func_element*) element -> left -> data) -> number
#define leftconstanta ((func_element*) element -> left -> data) -> constanta
#define leftvariable  ((func_element*) element -> left -> data) -> variable

#define righttype      ((func_element*) element -> right -> data) -> type
#define rightop        ((func_element*) element -> right -> data) -> op
#define rightnumber    ((func_element*) element -> right -> data) -> number
#define rightconstanta ((func_element*) element -> right -> data) -> constanta
#define rightvariable  ((func_element*) element -> right -> data) -> variable
