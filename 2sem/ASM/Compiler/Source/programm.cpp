#include "../Headers/programm.hpp"

Programm::Programm()
{
	source_path = 0;
	output_path = 0;
	source_text = 0;
	source_length = 0;
	libraries = 0;
	libraries_count = 0;
	definitions = 0;
	definitions_count = 0;
	source_tokens = 0;
	tokens_length = 0;
	compiled_text = 0;
	asm_listing = false;
	silent = false;
}

Programm::Programm(int argc, char* argv[])
{
	for (int argcounter = 1; argcounter < argc; argcounter++)
	{
		if (!strcmp(argv[argcounter], "-o") or !strcmp(argv[argcounter], "--output"))
			output_path = argv[++argcounter];
		
		else if (!strcmp(argv[argcounter], "-l") or !strcmp(argv[argcounter], "--listing"))
			asm_listing = true;
		
		else if (!strcmp(argv[argcounter], "-s") or !strcmp(argv[argcounter], "--silent"))
			silent = true;
		
		else
			source_path = argv[argcounter];
	}
	
	if (not output_path)
		output_path = "a.out";
		
	source_text = 0;
	source_length = 0;
	libraries = 0;
	libraries_count = 0;
	definitions = 0;
	definitions_count = 0;
	source_tokens = 0;
	tokens_length = 0;
	compiled_text = 0;
	
	if (not asm_listing)
		asm_listing = false;
		
	if (not silent)
		silent = false;
}


Programm::~Programm()
{
	if (source_text)
		delete source_text;
		
	if (source_tokens)
		delete source_tokens;
		
	if (compiled_text)
		delete compiled_text;
}




int Programm::readSource()
{
	if (strcmp(source_path + (strlen(source_path) - 5), ".jaul"))
	{
		printf("\x1b[1;31mError\x1b[0m: file \x1b[1m\"%s\"\x1b[0m extension not supported.\n", source_path);
		exit(WRONG_EXTENSION);
	}
	
	FILE* source_file = fopen(source_path, "r");
	
	if (not source_file)
	{
		printf("\x1b[1;31mError\x1b[0m: file \x1b[1m\"%s\"\x1b[0m not exist.\n", source_path);
		exit(FILE_NOT_EXIST);
	}
	
	fseek(source_file, 0, SEEK_END);
	source_length = ftell(source_file);
	rewind(source_file);

	source_text = new char[source_length + 1];
	(source_text)[source_length] = '\0';

	source_length = fread(source_text, sizeof(char), source_length, source_file);
	
	fclose(source_file);

	if (not silent)
		printf("\n\x1b[1;32m%lld\x1b[0;32m symbols readed from \x1b[1;32m%s\x1b[0m\n------------ \x1b[1;32mSOURCE CODE\x1b[0m ------------\n%s\n------------- \x1b[1;32mEND SOURCE\x1b[0m ------------\n\n", this -> source_length, this -> source_path, this -> source_text);
	
	return source_length;
}


unsigned long long int Programm::_calcNewLength()
{
	unsigned long long int new_length = 0;
	for (int counter = 0; counter < libraries_count; counter++)
		new_length = source_length - (8 + strlen(libraries[counter].libname)) + libraries[counter].liblength;
		
	return new_length;
}


int Programm::_importLibraries()
{
	libraries_count = strcount(source_text, "@import");
	libraries = new Library[libraries_count];
	
	int library_counter = 0;
	char* _source_text = source_text;
	bool flag = true;
	while (flag)
	{
		_source_text = strstr(_source_text, "@import");
		if (_source_text == nullptr)
			flag = false;
		else
		{
			_source_text += 8;
			int counter = 0;
			while (_source_text[counter] != ' ' and _source_text[counter] != '\n')
			{
				((libraries[library_counter]).libname)[counter] = _source_text[counter];
				counter++;
			}
			
			(libraries[library_counter]).readLibrary();
			
			if (not silent)
				printf("\x1b[1;34m\"%s\"\x1b[0m library imported\n", (libraries[library_counter]).libname);
			
			library_counter++;
		}
	}
	
	return library_counter;
}


int Programm::_setDefinitions()
{
	definitions_count = strcount(source_text, "@define");
	definitions = new Definition[definitions_count];
	
	int definition_counter = 0;
	char* _source_text = source_text;
	bool flag = true;
	while (flag)
	{
		_source_text = strstr(_source_text, "@define");
		if (_source_text == nullptr)
			flag = false;
		else
		{
			_source_text += 8;
			int counter = 0;
			while (_source_text[counter] != ' ' and _source_text[counter] != '\n')
				counter++;
				
			(definitions[definition_counter]).defname = new char[counter + 1]{0};
			(definitions[definition_counter]).defname_length = counter;
			
			counter = 0;
			while (_source_text[counter] != ' ' and _source_text[counter] != '\n')
			{
				(definitions[definition_counter]).defname[counter] = _source_text[counter];
				counter++;
			}
			
			_source_text += (counter + 1);
			counter = 0;
			
			while (_source_text[counter] != '\n')
				counter++;
				
			(definitions[definition_counter]).defstatement = new char[counter + 1]{0};
			(definitions[definition_counter]).defstatement_length = counter;
			
			counter = 0;
			while (_source_text[counter] != '\n')
			{
				(definitions[definition_counter]).defstatement[counter] = _source_text[counter];
				counter++;
			}
			
			if (not silent)
				printf("Definition \x1b[1;34m\"%s\"\x1b[0m means \x1b[1;34m\"%s\"\x1b[0m in this scope\n", (definitions[definition_counter]).defname, (definitions[definition_counter]).defstatement);
			
			definition_counter++;
		}
	}
	
	return definition_counter;
}


int Programm::preprocessor()
{
	_importLibraries();
	_setDefinitions();
	unsigned long long int new_length = _calcNewLength();
	char* preprocessed_source_text = new char[new_length];
	unsigned long long int counter = 0;
	while(source_text[counter] != '\0')
	{
		counter++;
	}
	
	source_length = new_length;
	free(source_text);
	source_text = preprocessed_source_text;
	return 0;
}






Definition::Definition()
{
	defname = 0;
	defstatement = 0;
	defname_length = 0;
	defstatement_length = 0;
}

Definition::~Definition()
{
	if (defname)
		delete defname;
	if (defstatement)
		delete defstatement;
}



Library::Library()
{
	libname = new char[30]{0};
	libtext = 0;
	liblength = 0;
}

Library::~Library()
{
	if (libname)
		delete libname;
	if (libtext)
		delete libtext;
}

int Library::readLibrary()
{
	if (strcmp(libname + (strlen(libname) - 5), ".jaul"))
	{
		printf("\x1b[1;31mError\x1b[0m: file \x1b[1m\"%s\"\x1b[0m is not a library.\n", libname);
		exit(WRONG_EXTENSION);
	}
	
	FILE* source_file = fopen(libname, "r");
	
	if (not source_file)
	{
		printf("\x1b[1;31mError\x1b[0m: library \x1b[1m\"%s\"\x1b[0m not exist.\n", libname);
		exit(FILE_NOT_EXIST);
	}
	
	fseek(source_file, 0, SEEK_END);
	liblength = ftell(source_file);
	rewind(source_file);

	libtext = new char[liblength + 1];
	(libtext)[liblength] = '\0';

	liblength = fread(libtext, sizeof(char), liblength, source_file);
	
	fclose(source_file);

	return liblength;
}


int strcount(char* str, const char* expression)
{
	int counter = 0;
	char* _str = str;
	bool flag = true;
	while (flag)
	{
		_str = strstr(_str, expression);
		if (_str == nullptr)
			flag = false;
		else
		{
			counter++;
			_str += 1;
		}
	}
	
	return counter;
}





// end;
