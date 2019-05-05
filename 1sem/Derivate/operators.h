#define dR derivate_branch (func_derivated, element -> right)
#define dL derivate_branch (func_derivated, element -> left)
#define cR copy_branch (func_derivated, element -> right)
#define cL copy_branch (func_derivated, element -> left)

#define meow


//!   DEF_OP( operator, num, name, code )

DEF_OP ("+",        1,      plus,       {   new_node = (*dL + *dR);                                      })
DEF_OP ("-",        2,      minus,      {   new_node = (*dL + *dR);                                      })
DEF_OP ("*",        3,      mul,        {   new_node = (*(*dL * *cR) + *(*cL * *dR));                    })
DEF_OP ("/",        4,      div,        {   new_node = (*(*(*dL * *cR) - *(*cL * *dR)) / *(*cR * *cR));  })
DEF_OP ("^",        5,      pow,        {})
DEF_OP ("sin",      6,      sin,        {})
DEF_OP ("cos",      7,      cos,        {})
