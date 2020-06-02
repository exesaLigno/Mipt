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

	short int type = getType(this -> name);
	
	if (type == ERRTYPE)
		return UNSUPPORTED_FILE_EXTENSION;
	
	else
		this -> source_type = type;
		
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
	char* extension = strchr(name, '.');
	
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

int getDefCount()
{
	int def_count = 0

	char* finded = strstr(this -> text, "define ");
	
	while (finded != nullptr)
	{
		def_count++;
		finded = strstr(++finded, "define ");
	}
	
	return def_count;
}

Source* Source::getDefinition()	// Rewrite to \n processing
{
	this -> text_pointer = strstr(this -> text_pointer, "define ");
	this -> text_pointer += 7;
	
	Source* definition = new Source;
	
	definition -> name = new char[strchr(this -> text_pointer, ' ') - this -> text_pointer + 1]{0};
	strncpy(definition -> name, this -> text_pointer, strchr(this -> text_pointer, ' ') - this -> text_pointer);
	
	this -> text_pointer = strchr(this -> text_pointer, ' ') - this -> text_pointer + 1
}

int Source::substitute(const Source& source)
{
	if (source.source_type != DEFINITION)
		return NOT_SUBSTITUTABLE_TYPE;
		
	int name_length = strlen(source.name);
	int substitution_length = source.text_length;
		
	long long int new_text_length = this -> text_length;
	
	char* finded = strstr(this -> text, source.name);
	
	while (finded != nullptr)
	{
		new_text_length = new_text_length - name_length + substitution_length;
		finded = strstr(++finded, source.name);
	}
	
	char* new_text = new char[new_text_length + 1]{0};
	char* _new_text = new_text;
	char* _text = this -> text;
	
	finded = strstr(this -> text, source.name);
	
	while (finded != nullptr)
	{
		strncat(_new_text, _text, finded - _text);
		_new_text += finded - _text;
		_text = finded + name_length;
		
		strcat(_new_text, source.text);
		_new_text += substitution_length;
		
		finded = strstr(finded, source.name);
	}
	
	delete[] this -> text;
	
	this -> text = new_text;
	this -> text_length = new_text_length;
	
	return OK;
}

void Source::rewind()
{
	this -> text_pointer = this -> text;
}




