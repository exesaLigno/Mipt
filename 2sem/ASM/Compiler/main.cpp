#include <fstream>
#include <unistd.h>
#include "Headers/programm.hpp"


/*----------------------------------------------|
| TODO 	In programm.cpp - symbol by symbol		|
|		printing								|
|----------------------------------------------*/

#include "Headers/debug.hpp"

int main(int argc, char* argv[])
{
	Programm programm(argc, argv);
	programm.readSource();
	programm.preprocessor();
	DEBUG
	//programm.makeTokens();
	//DEBUG
	//programm.makeTree();
	//DEBUG
	//while(programm.optimize());
	//DEBUG
	//programm.compile();
	//DEBUG
	//programm.write();
	//DEBUG
	
    return 0;
}
