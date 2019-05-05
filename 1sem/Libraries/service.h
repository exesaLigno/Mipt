#define NOT_IMPLEMENTED assert (!"This function is not implemented yet!");
#define NOT_DEBUGGED printf ("You using function that not works at all (call from %d line)\n", __LINE__);


#define DEBUG( code ) printf (">Debug out from %d line: ", __LINE__); code; printf ("\n");
//#define DEBUG printf (">Debug out from %d line\n", __LINE__);


#define SWITCH_OFF if(0)
#define SWITCH_ON if(1)
