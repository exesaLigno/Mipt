#pragma once

#include <cstring>
#include <fstream>
#include <iostream>

#include "source.hpp"

class Compiler
{
  public:	// protected
  
	//------------- SETTINGS -------------//
	char** source_pathes = NULL;
	int source_count = 0;
	char* output_path = NULL;
	
	bool verbose = false;
	
	bool only_preprocess = false;
	bool nasm_listing = false;
	bool virtual_compilation = false;
	
	short optimization_level = 2;
	
	bool run_virtual = false;
	
	bool correct_usage = true;
	
	//------------------------------------//
	
	Source** source_list;
		
	
  public:
	
	enum Errors
	{
		OK,
		UNKNOWN_PARAMETER,
		ERRORS_COUNT
	};
	
	Compiler(int argc, char* argv[]);
	~Compiler();
	
	void addPath(const char* source_path);
	void showSettings();
	void showHelp();
	bool validate();
	
	void readSource();
	void showSource();
	
	void makeAst();
};

