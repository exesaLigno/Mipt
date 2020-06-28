#include "../Headers/source.hpp"


Source::Source()
{
	this -> name = NULL;
	this -> text = NULL;
	this -> text_length = 0;
	this -> text_pointer = NULL;
	this -> source_type = 0;
}

Source::Source(const char* name)
{
	short int type = getType(name);
	
	this -> source_type = type;
	
	this -> name = new char[strlen(name) + 1]{0};
	strcpy(this -> name, name);
	
	this -> text = NULL;
	this -> text_length = 0;
	
	this -> text_pointer = NULL;
	
	this -> source_type = 0;
}

Source::~Source()
{
	if (this -> name != NULL)
		delete[] this -> name;
		
	if (this -> text != NULL)
		delete[] this -> text;
}

short int Source::open()
{
	if (this -> source_type == DEFINITION)
		return OPENNING_PROHIBITED;
		
	FILE* source_file = fopen(this -> name, "r");	// Trying to open file
	
	if (not source_file)	// Trying to find file in std library path
	{
		// TODO config reading from Jaul.conf
		char* std_name = new char[strlen(this -> name) + strlen(".std/") + 1]{0};
		strcpy(std_name, ".std/");
		strcat(std_name, this -> name);
		source_file = fopen(std_name, "r");
		delete[] std_name;
	}
	
	if (not source_file)
	{
		printf("\x1b[1;31mError\x1b[0m: file \x1b[1m\"%s\"\x1b[0m not exist.\n", this -> name);
		return FILE_NOT_EXIST;
	}
	
	fseek(source_file, 0, SEEK_END);
	this -> text_length = ftell(source_file);
	rewind(source_file);

	this -> text = new char[this -> text_length + 1]{0};
	
	this -> text_pointer = this -> text;

	this -> text_length = fread(this -> text, sizeof(char), this -> text_length, source_file);
	
	fclose(source_file);
	
	return OK;
}

short int Source::getType(const char* name)
{
	const char* extension = strchr(name, '.');
	
	if (extension == nullptr)
		return OBJ_SOURCE;
		
	else
	{
		if (!strcmp(extension, ".jaul"))
			return JAUL_SOURCE;
			
		else if (!strcmp(extension, ".s"))
			return NASM_SOURCE;
			
		else if (!strcmp(extension, ".jaulv"))
			return VIRTUAL_EXECUTABLE;
			
		else
			return ERRTYPE;
	}
}


void Source::print()
{
	if (this -> name)
		printf("\n\x1b[1;32m%s\x1b[0m\n\n", this -> name);
		
	if (this -> text)
		printf("%s\n", this -> text);
	else 
		printf("\x1b[2mEmpty\x1b[0m\n");
}


