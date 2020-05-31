Source::Source(const char* name)
{
	this -> name = new char[strlen(name) + 1]{0};
	strcpy(this -> name, name);
	
	this -> text = NULL;
	this -> text_length = NULL;
	
	this -> source_type = NULL;
}

Source::~Source()
{
	if (this -> name)
		delete[] this -> name;
		
	if (this -> text)
		delete[] this -> text;
}

int open()
{
	if (this -> source_type != JAUL_SOURCE or this -> source_type != NASM_SOURCE or this -> source_type != OBJ_SOURCE)
		return OPENNING_PROHIBITED;
		
	if (0) // Сравнение расширения с текущим типом и возврат кода ошибки
		return UNSUPPORTED_FILE_EXTENSION;
		
	FILE* source_file = fopen(this -> name, "r");
	
	// Поиск в стандартной директории библиотек
	
	if (not source_file)
	{
		printf("\x1b[1;31mError\x1b[0m: file \x1b[1m\"%s\"\x1b[0m not exist.\n", source_path);
		exit(FILE_NOT_EXIST);
	}
	
	fseek(source_file, 0, SEEK_END);
	this -> text_length = ftell(source_file);
	rewind(source_file);

	this -> text = new char[this -> text_length + 1]{0};

	this -> text_length = fread(this -> text, sizeof(char), this -> text_length, source_file);
	
	fclose(source_file);
}

