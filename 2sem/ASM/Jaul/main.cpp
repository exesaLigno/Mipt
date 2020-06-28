#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Headers/compiler.hpp"

//#define release



/*----------------------------------------------|
| TODO 	In programm.cpp - symbol by symbol		|
|		printing								|
|----------------------------------------------*/

#include "Headers/debug.hpp"

int main(int argc, char* argv[])
{
	Compiler compiler(argc, argv);
	
	if (not compiler.validate())
	{
		compiler.showHelp();
		return Compiler::UNKNOWN_PARAMETER;
	}
	compiler.showSettings();
	
	compiler.readSource();
	compiler.showSource();
	
	compiler.makeAst();
	
    return 0;
}








// end;
