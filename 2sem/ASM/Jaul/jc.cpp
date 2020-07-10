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
	int status = Compiler::OK;
	
	Compiler compiler(argc, argv);

	if (compiler.mode() == Compiler::ERROR)
	{
		compiler.showHelp();
		status = Compiler::UNKNOWN_PARAMETER;
	}
	
	else if (compiler.mode() == Compiler::HELP)
	{
		compiler.showHelp();
		status = Compiler::OK;
	}
	
	else
	{
		compiler.showSettings();
		
		compiler.readSource();
		compiler.showSource();
		
		compiler.makeAST();
		compiler.dumpAST();
	}

    return status;
}








// end;
