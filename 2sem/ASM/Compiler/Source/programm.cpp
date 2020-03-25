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
	unsigned long long int new_length = source_length;
	
	for (int counter = 0; counter < libraries_count; counter++)
		new_length += -(8 + strlen(libraries[counter].libname)) + libraries[counter].libtext_length;
	
	for (int counter = 0; counter < definitions_count; counter++)
	{
		int defcount = strcount(source_text, (definitions[counter]).defname);
		int lendelta = (definitions[counter]).defstatement_length - (definitions[counter]).defname_length;
		new_length += lendelta * defcount;
		new_length -= (10 + (definitions[counter]).defstatement_length + (definitions[counter]).defname_length);
	}
		
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
			
			(libraries[library_counter]).libname_length = counter;
			
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
	
	if ((not silent) and new_length != source_length)
		printf("Source buffer expended from %lld to %lld\n", source_length, new_length);
		
	char* preprocessed_source_text = new char[new_length]{0};
	char* _preprocessed_source_text = preprocessed_source_text;
	char* _source_text = source_text;
	
	while(*_source_text != '\0')
	{
		
		bool solved = false;
		
		for (int counter = 0; counter < definitions_count; counter++)
		{
			if (not strncmp(_source_text, (definitions[counter]).defname, (definitions[counter]).defname_length) and isSpace(*(_source_text - 1)) and isSpace(*(_source_text + (definitions[counter]).defname_length)))
			{
				strcpy(_preprocessed_source_text, (definitions[counter]).defstatement);
				_preprocessed_source_text += (definitions[counter]).defstatement_length;
				_source_text += (definitions[counter]).defname_length;
				solved = true;
				break;
			}			
		}
		
		if (solved)
			continue;
		
		for (int counter = 0; counter < libraries_count; counter++)
		{
			if (not strncmp(_source_text + 8, (libraries[counter]).libname, strlen((libraries[counter]).libname)))
			{
				strcpy(_preprocessed_source_text, (libraries[counter]).libtext);
				_preprocessed_source_text += (libraries[counter]).libtext_length;
				_source_text += ((libraries[counter]).libname_length + 8);
				solved = true;
				break;
			}			
		}
		
		if (solved)
			continue;
		
		if (*_source_text == '#' or *_source_text == '@')
		{
			while (*_source_text != '\n' and *_source_text != '\0')
				_source_text++;
		}
		
		*_preprocessed_source_text = *_source_text;
		
		_source_text++;
		_preprocessed_source_text++;
	}
	
	source_length = new_length;
	free(source_text);
	source_text = preprocessed_source_text;
	
	if (not silent)
		printf("\n\x1b[1;32m%lld\x1b[0;32m symbols preprocessed\n------------ \x1b[1;32mSOURCE CODE\x1b[0m ------------\n%s\n------------- \x1b[1;32mEND SOURCE\x1b[0m ------------\n\n", source_length, source_text);
	
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
	libtext_length = 0;
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
	libtext_length = ftell(source_file);
	rewind(source_file);

	libtext = new char[libtext_length + 1];
	(libtext)[libtext_length] = '\0';

	libtext_length = fread(libtext, sizeof(char), libtext_length, source_file);
	
	fclose(source_file);

	return libtext_length;
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

bool isSpace(char symbol)
{
	return (symbol == ' ' or symbol == '\n' or symbol == '\t' or symbol == '\0');
}







// end;
