#include "../Headers/compiler.hpp"

Compiler::Compiler(int argc, char* argv[])
{
	if (argc <= 1)
		this -> correct_usage = false;
	
	else
	{
		for (int counter = 1; counter < argc; counter++)
		{
			if (!strcmp("-v", argv[counter]) or !strcmp("--verbose", argv[counter]))
				this -> verbose = true;
			
			else if (!strcmp("-p", argv[counter]) or !strcmp("--only-preprocess", argv[counter]))
				this -> only_preprocess = true;
			
			else if (!strcmp("-l", argv[counter]) or !strcmp("--nasm-listing", argv[counter]))
				this -> nasm_listing = true;
				
			else if (!strcmp("--run", argv[counter]))
				this -> run_virtual = true;
				
			else if (!strcmp("--virtual", argv[counter]))
				this -> virtual_compilation = true;
			
			else if (!strcmp("-o", argv[counter]))
			{
				counter++;
				this -> output_path = new char[strlen(argv[counter]) + 1]{0};
				strcpy(this -> output_path, argv[counter]);
			}
			
			else if (!strcmp("-o0", argv[counter]) or !strcmp("-o1", argv[counter]) or !strcmp("-o2", argv[counter]))
				this -> optimization_level = argv[counter][2] - '0';
			
			else if (argv[counter][0] != '-')
				this -> addPath(argv[counter]);
				
			else
			{
				printf("\x1b[1;31mError:\x1b[0m Unknown parameter \x1b[3m\'%s\'\x1b[0m\n", argv[counter]);
				this -> correct_usage = false;
				break;
			}
		}
	}
}



Compiler::~Compiler()
{
	if (this -> source_pathes)
	{
		for (int counter = 0; counter < this -> source_count; counter++)
		{
			if ((this -> source_pathes)[counter])
				delete[] (this -> source_pathes)[counter];
		}
		delete[] this -> source_pathes;
	}
	
	if (this -> source_list)
	{
		for (int counter = 0; counter < this -> source_count; counter++)
		{
			if ((this -> source_list)[counter])
				delete (this -> source_list)[counter];
		}
		delete[] this -> source_list;
	}
		
	if (this -> output_path)
		delete[] this -> output_path;
}



void Compiler::addPath(const char* source_path)
{
	int new_source_count = this -> source_count + 1;
	char** new_source_pathes = new char*[new_source_count];
	
	for (int counter = 0; counter < this -> source_count; counter++)
		new_source_pathes[counter] = (this -> source_pathes)[counter];
	
	new_source_pathes[this -> source_count] = new char[strlen(source_path) + 1];
	strcpy(new_source_pathes[this -> source_count], source_path);
	
	delete[] this -> source_pathes;
	this -> source_pathes = new_source_pathes;
	this -> source_count = new_source_count;
}

void Compiler::showSettings()
{
	if (not verbose)
		return;
	
	printf("\n---------------\x1b[1;32m Compiler \x1b[0m---------------\n");
	
	if (this -> source_pathes)
	{
		printf("Source list: ");
		for (int counter = 0; counter < this -> source_count; counter++)
			printf("%s ", (this -> source_pathes)[counter]);
		printf("\n");
	}
	else
		printf("No sources listed!\n");
	
	if (this -> output_path)
		printf("Output path: %s\n", this -> output_path);
	else
		printf("Using default output path\n");
	
	printf("%s", this -> verbose ? "Verbose\n" : "");
	
	printf("%s", this -> only_preprocess ? "Preprocessor\n" : "");
	printf("%s", this -> nasm_listing ? "Nasm listing\n" : "");
	
	printf("%s", this -> virtual_compilation ? "Virtual compilation\n" : "");
	printf("%s", this -> run_virtual ? "Run virtual\n" : "");
	
	printf("Optimization level: %d\n", this -> optimization_level);
	
	printf("----------------------------------------\n\n");
}


void Compiler::showHelp()
{
	printf("Jaul Compiler (2020)\n");
	printf("Usage: ./jc [options] file...\n");
	printf("Options:\n");
	printf("  -v --verbose             Detailed compilation process output\n");
	printf("  -p --only-preprocess     Only preprocess code and save it\n");
	printf("  -l --nasm-listing        Generate NASM listing\n");
	printf("     --run                 Try to run virtual code on jvm\n");
	printf("     --virtual             Compile code to virtual executable\n");
	printf("  -o <file>                Write compiled code to \x1b[2m<file>\x1b[0m. If not specified, using \x1b[2ma.out\x1b[0m\n");
	printf("  -o#                      Optimization level, # = 0, 1 or 2\n");
}


bool Compiler::validate()
{
	return correct_usage;
}



void Compiler::readSource()
{
	this -> source_list = new Source*[this -> source_count];
	for (int counter = 0; counter < this -> source_count; counter++)
	{
		(this -> source_list)[counter] = new Source((this -> source_pathes)[counter]);
		(this -> source_list)[counter] -> open();
	}
}


void Compiler::showSource()
{
	if (not verbose)
		return;
		
	if (this -> source_list)
	{
		for (int counter = 0; counter < this -> source_count; counter++)
		{
			(this -> source_list)[counter] -> print();
			printf("\n-----------------------\n\n");
		}
	}
}


void Compiler::makeAst()
{
	return;
}



