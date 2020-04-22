#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Headers/programm.hpp"
#include "Headers/settings.hpp"


/*----------------------------------------------|
| TODO 	In programm.cpp - symbol by symbol		|
|		printing								|
|----------------------------------------------*/

#include "Headers/debug.hpp"

int main(int argc, char* argv[])
{
	Settings settings(argc, argv);	// 1 error
	DEBUG
	Programm programm;				// 0 errors
	DEBUG
	programm.readSource(&settings); // 3 errors
	
	programm.preprocessor(&settings); // 25 errors
	
	
	programm.write(&settings);
	
	//programm.makeTokens();
	//programm.makeTree();
	//while(programm.optimize());
	//programm.compile();
	//programm.write();
	
    return 0;
}








// end;
