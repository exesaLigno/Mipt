#pragma once

#include <cstring>
#include <fstream>
#include <iostream>

#include "source.hpp"
#include "ast.hpp"

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
	
	AST ast;
	bool ast_associated = false;
		
	
  public:
  
  	enum Modes
  	{
  		COMPILATION,
  		HELP,
  		ERROR
  	};
	
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
	bool mode();
	
	void readSource();
	void showSource();
	
	void makeAST();
	void dumpAST();
};

