#include "../Headers/programm.hpp"

Programm::Programm()
{
	text = 0;
	text_length = 0;
	//source_tokens = 0;
	//tokens_length = 0;
	//programm_tree;
}


Programm::~Programm()
{
	if (this -> text)
		free(text);


	//if (this -> source_tokens)
	//	delete source_tokens;
}




int Programm::readSource(const Settings* settings)
{
	char* source_path = settings -> source_path;
	
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
	this -> text_length = ftell(source_file);
	rewind(source_file);

	this -> text = (char*) calloc(this -> text_length + 1, sizeof(char));
	(this -> text)[this -> text_length] = '\0';

	this -> text_length = fread(this -> text, sizeof(char), this -> text_length, source_file);
	
	fclose(source_file);

	if (not settings -> silent)
		printf("\n\x1b[1;32m%lld\x1b[0;32m symbols readed from \x1b[1;32m%s\x1b[0m\n------------ \x1b[1;32mSOURCE CODE\x1b[0m ------------\n%s\n------------- \x1b[1;32mEND SOURCE\x1b[0m ------------\n\n", this -> text_length, source_path, this -> text);
	
	return this -> text_length;
}


int Programm::preprocessor(const Settings* settings)
{
	DEBUG
	while (importLibraries());
	DEBUG
	setDefinitions();
	DEBUG
	deleteComments();
	DEBUG
	if (not settings -> silent)
		printf("\n\x1b[1;32m%lld\x1b[0;32m symbols preprocessed\n------------ \x1b[1;32mSOURCE CODE\x1b[0m ------------\n%s\n------------- \x1b[1;32mEND SOURCE\x1b[0m ------------\n\n", this -> text_length, this -> text);
	DEBUG
	return 0;
}


int Programm::importLibraries()
{
	int imports_count = strcount(this -> text, "@import");
	DEBUG
	if (imports_count == 0)
		return 0;
	DEBUG
	Library* libraries = (Library*) calloc(imports_count, sizeof(Library));
	DEBUG
	int new_length = this -> text_length;
	char* _text = this -> text;
	DEBUG
	for (int counter = 0; counter < imports_count; counter++)
	{
		_text = strstr(_text, "@import") + 8;
		(libraries[counter]).libname_length = std::min(strchr(_text, ' '), strchr(_text, '\n')) - _text;
		(libraries[counter]).libname = (char*) calloc ((libraries[counter]).libname_length + 1, sizeof(char));
		strncpy((libraries[counter]).libname, _text, (libraries[counter]).libname_length);
		(libraries[counter]).readLibrary();
		new_length = new_length - (8 + (libraries[counter]).libname_length) + (libraries[counter]).libtext_length;
	}
	DEBUG
	this -> text_length = new_length;
	char* old_text = this -> text;
	char* _old_text = old_text;
	this -> text = (char*) calloc(this -> text_length, sizeof(char));
	_text = this -> text;
	
	int lib_counter = 0;
	
	while (*_old_text)
	{
		if (!strncmp(_old_text, "@import", 7))
		{
			strcpy(_text, (libraries[lib_counter]).libtext);
			_text += (libraries[lib_counter]).libtext_length;
			_old_text += (8 + (libraries[lib_counter]).libname_length);
			lib_counter++;
		}
		
		else
		{
			*_text = *_old_text;
			_text++;
			_old_text++;
		}
	}
	
	free(old_text);
	
	free(libraries);			// TODO add recursive including check
	
	return imports_count;
}


int Programm::setDefinitions()		// TODO fix replacing middle of word (word borders checking)
									// TODO fix memory errors
{
	int definitions_count = strcount(this -> text, "@define");
	if (definitions_count == 0)
		return 0;
		
	Definition* definitions = (Definition*) calloc(definitions_count, sizeof(Definition));
	
	int new_length = this -> text_length;
	char* _text = this -> text;
	
	for (int counter = 0; counter < definitions_count; counter++)
	{
		_text = strstr(_text, "@define") + 8;
		(definitions[counter]).defname_length = std::min(strchr(_text, ' '), strchr(_text, '\n')) - _text;
		
		(definitions[counter]).defname = (char*) calloc((definitions[counter]).defname_length, sizeof(char));
		strncpy((definitions[counter]).defname, _text, (definitions[counter]).defname_length);
		
		_text += (definitions[counter]).defname_length + 1;
		(definitions[counter]).defstatement_length = strchr(_text, '\n') - _text;
		
		(definitions[counter]).defstatement = (char*) calloc((definitions[counter]).defstatement_length, sizeof(char));
		strncpy((definitions[counter]).defstatement, _text, (definitions[counter]).defstatement_length);
		
		new_length = new_length + (((definitions[counter]).defstatement_length - (definitions[counter]).defname_length) \
									* (strcount(this -> text, (definitions[counter]).defname) - 1) - \
									((definitions[counter]).defstatement_length + (definitions[counter]).defname_length) + 9);
	}
	
	this -> text_length = new_length;
	char* old_text = this -> text;
	char* _old_text = old_text;
	this -> text = (char*) calloc(this -> text_length, sizeof(char));
	_text = this -> text;

	while (*_old_text)
	{
		if (!strncmp(_old_text, "@define", 7))
		{
			while(*_old_text != '\n')
				_old_text++;
		}
		
		bool defstatement_placed = false;
		for (int counter = 0; counter < definitions_count; counter++)
		{
			if (!strncmp(_old_text, definitions[counter].defname, definitions[counter].defname_length))
			{
				strncpy(_text, definitions[counter].defstatement, definitions[counter].defstatement_length);
				_text += definitions[counter].defstatement_length;
				_old_text += definitions[counter].defname_length;
				defstatement_placed = true;
				break;
			}
		}
		
		if (not defstatement_placed)
		{
			*_text = *_old_text;
			_text++;
			_old_text++;
		}
	}
	
	free(old_text);

	free(definitions);
	
	return definitions_count;
}


int Programm::deleteComments()
{
	char* _text = this -> text;
	char* _old_text = this -> text;
	
	bool in_comment = false;
	while (*_old_text)
	{
		if (*_old_text == '#')
			in_comment = true;
		
		if (*_old_text == '\n')
			in_comment = false;
			
		if (in_comment)
			_old_text++;
			
		else
		{
			*_text = *_old_text;
			_text++;
			_old_text++;
		}
	}
	
	*_text = '\0';
	
	return 0;
}


int Programm::write(const Settings* settings)
{
	FILE* file = fopen("tested.out", "w");

	fprintf(file, "%s", this -> text);
	
	fclose(file);
	
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
		free(defname);
	if (defstatement)
		free(defstatement);
}



Library::Library()
{
	libname = 0;
	libname_length = 0;
	libtext = 0;
	libtext_length = 0;
}

Library::~Library()
{
	if (libname)
		free(libname);
	if (libtext)
		free(libtext);
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

	libtext = (char*) calloc(libtext_length + 1, sizeof(char));
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
