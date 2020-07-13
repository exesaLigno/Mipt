#pragma once

#include <cstring>
#include <fstream>
#include <iostream>

#include "source.hpp"
#include "binary.hpp"

/*!
 *	@brief Compiler class, created for organizing compiler work
 *	@date July 2020
 *	@bug Rarely can not compile JAUL source to AbstractSyntaxTree
 *
 *	Main compiler class, control all compiler processes and stores all compilation information and source/compiled code
 */
class Compiler
{
  public:	// protected
  
	//------------- SETTINGS -------------//
	char** source_pathes = nullptr;
	int source_count = 0;
	char* output_path = nullptr;
	
	bool verbose = false;
	
	bool only_preprocess = false;
	bool nasm_listing = false;
	bool virtual_compilation = false;
	bool obj_generation = false;
	
	bool show_help = false;
	
	short optimization_level = 2;
	
	bool correct_usage = true;
	
	//------------------------------------//
	
	Source** source_list = nullptr;
	Binary** binary_list = nullptr;
		
	
  public:
  
	//! Compiler work modes
  	enum Modes
  	{
  		COMPILATION,	///< Compiler working in default mode
  		HELP,			///< Just show help and exit
  		ERROR			///< Compiler catch an error and exited urgently
  	};
	
	//! Errors list
	enum Errors
	{
		OK,					///< No error, all goes well
		UNKNOWN_PARAMETER,	///< Unknown parameter in terminal call
		ERRORS_COUNT
	};
	
	Compiler(int argc, char* argv[]);
	~Compiler();
	
	void addPath(const char* source_path);
	void showSettings();
	void showHelp();
	bool mode();
	
	void readSource();
	void showSource();
	
	void makeAST();
	void dumpAST();
	
	void createBinaries();
	void compile();
	void assemble();
	void write();
};

