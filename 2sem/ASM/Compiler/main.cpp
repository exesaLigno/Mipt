#include <fstream>
#include "Headers/programm.hpp"

#ifndef release
	#define DEBUG printf(">> debug from <%s::%d>\n", __FILE__, __LINE__);
#else
	#define DEBUG
#endif

int main(int argc, char* argv[])
{
	DEBUG
	Programm programm(argc, argv);
	DEBUG
	programm.readSource();
	DEBUG
	programm.makeTokens();
	DEBUG
	programm.makeTree();
	DEBUG
	while(programm.optimize());
	DEBUG
	programm.compile();
	DEBUG
	programm.write();
	DEBUG
	
    return 0;
}
