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
	Settings settings(argc, argv);

	Programm programm;

	programm.readSource(&settings);
	programm.preprocessor(&settings);
	
	if (settings.only_preprocess)
	{
		programm.write(&settings);
		return 0;
	}
	
	programm.makeTree(&settings);
	//while(programm.optimize());
	//programm.compile();
	//programm.write();
	
    return 0;
}








// end;
