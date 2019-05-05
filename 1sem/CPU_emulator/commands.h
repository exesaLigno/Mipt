#define mpush( element )    push (&core1.proc_stack, element)
#define mpop                pop  (&core1.proc_stack)

#define cpush( adress )     push (&core1.call_stack, adress)
#define cpop                pop  (&core1.call_stack)
#define parametr            program [IP++]

#define ram                 core1.RAM
#define reg                 core1.registers

#define meow





///DEF_CMD (name, num, code)


DEF_CMD (PUSH,   push,  1,  {mpush (parametr);                                  })

DEF_CMD (POP,    pop,   2,  {mpop;                                              })

DEF_CMD (PUSHR,  push,  3,  {mpush (reg [(int) parametr]);                      })

DEF_CMD (POPR,   pop,   4,  {reg [(int) parametr] = mpop;                       })

DEF_CMD (PUSHM,  push,  5,  {mpush (ram [(int) parametr]);                      })

DEF_CMD (POPM,   pop,   6,  {ram [(int) parametr] = mpop;                       })

DEF_CMD (PUSHRM, push,  7,  {mpush (ram [(int) reg [(int) parametr]]);          })

DEF_CMD (POPRM,  pop,   8,  {ram [(int) reg [(int) parametr]] = mpop;           })

DEF_CMD (ADD,    add,   9,  {mpush (mpop + mpop);                               })

DEF_CMD (SUB,    sub,   10, {mpush (mpop - mpop);                               })

DEF_CMD (MUL,    mul,   11, {mpush (mpop * mpop);                               })

DEF_CMD (DIV,    div,   12, {mpush (mpop / mpop);                               })

DEF_CMD (OUT,    out,   13, {printf ("%lg\n", mpop);                            })

DEF_CMD (END,    end,   14, {printf ("END\n");                                  })

DEF_CMD (JMP,    jmp,   15, {IP = program [IP];                                 })

DEF_CMD (JE,     je,    16, {if (mpop == mpop) IP = program [IP];
                             else IP++;                                         })

DEF_CMD (JNE,    jne,   17, {if (mpop != mpop) IP = program [IP];
                             else IP++;                                         })

DEF_CMD (JA,     ja,    18, {if (mpop > mpop) IP = program [IP];
                             else IP++;                                         })

DEF_CMD (JAE,    jae,   19, {if (mpop >= mpop) IP = program [IP];
                             else IP++;                                         })

DEF_CMD (JB,     jb,    20, {if (mpop < mpop) IP = program [IP];
                             else IP++;                                         })

DEF_CMD (JBE,    jbe,   21, {if (mpop <= mpop) IP = program [IP];
                             else IP++;                                         })

DEF_CMD (CALL,   call,  22, {cpush (IP + 1);
                             IP = program [IP];                                 })

DEF_CMD (RET,    ret,   23, {IP = cpop;                                         })

DEF_CMD (FLAG,   flag,  24, {printf ("flag detected\n");                        })

DEF_CMD (SWAP,   swap,  25, {meow;                                              })

DEF_CMD (IN,     in,    26, {double a = 0;
                             scanf ("%lg", &a);
                             mpush (a);                                         })

DEF_CMD (SQRT,   sqrt,  27, {mpush (sqrt (mpop));                               })

DEF_CMD (SQR,    sqr,   28, {mpush (pow (mpop, 2));                             })

DEF_CMD (POW,    pow,   29, {mpush (pow (mpop, mpop));                          })

/*

CMD_PUSH = 1,
    CMD_POP,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_OUT,
    CMD_END,
    CMD_PUSHR,
    CMD_POPR,
    CMD_JMP,

DEF_CMD (push, 1,  {PUSH (program [IP + 1]);})
DEF_CMD (pop,  2,  {POP;})
DEF_CMD (add,  3,  {PUSH (POP + POP);})
DEF_CMD (sub,  4,  {PUSH (POP - POP);})
DEF_CMD (mul,  5,  {PUSH (POP * POP);})
DEF_CMD (div,  6,  {PUSH (POP / POP);})
DEF_CMD (sqrt, 7,  {})
DEF_CMD (pow,  8,  {})
DEF_CMD (sin,  9,  {})
DEF_CMD (cos,  10, {})
DEF_CMD (out,  11, {printf ("%lg", POP);})
DEF_CMD (jmp,  12, {})
DEF_CMD (je,   13, {})
DEF_CMD (jne,  14, {})
DEF_CMD (ja,   15, {})
DEF_CMD (jae,  16, {})
DEF_CMD (jb,   17, {})
DEF_CMD (jbe,  18, {})
DEF_CMD (call, 19, {})
DEF_CMD (ret,  20, {})
*/
