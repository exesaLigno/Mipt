#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Headers/compiler.hpp"

/*!
 *	@file
 *	@brief Main file of JAUL compiler
 *	@mainpage JCP - JAUL Compiler Pack
 */


/*----------------------------------------------|
| TODO 	In programm.cpp - symbol by symbol		|
|		printing								|
|----------------------------------------------*/

#include "Headers/debug.hpp"


/*!
 *	Main logic of compiler
 *	@param[in] argc Count of terminal arguments
 *	@param[in] argv Terminal arguments
 *	@return Status of compiler work
 */
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
		compiler.createBinaries();
		compiler.compile();
	}

    return status;
}








// end;
