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
	
	compiler.showHelp();
	compiler.showSettings();
	
	if (compiler.status == Compiler::OK)
	{		
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
