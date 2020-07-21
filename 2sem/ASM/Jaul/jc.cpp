#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Headers/compiler.hpp"

/*!
 *	@file jc.cpp
 *	@author Карцев Вадим
 *	@brief Основной файл компилятора
 */


/*----------------------------------------------|
| TODO 	In programm.cpp - symbol by symbol		|
|		printing								|
|----------------------------------------------*/

#include "Headers/debug.hpp"


/*!
 *	Основная логика компилятора
 *	@param[in] argc Количество параметров командной строки
 *	@param[in] argv Параметры командной строки
 *	@return Статус работы компилятора
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
		compiler.compile();
		compiler.write();
	}

    return status;
}








// end;
