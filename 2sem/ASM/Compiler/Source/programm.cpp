#include "../Headers/programm.hpp"

Programm::Programm()
{
	this -> source_path = 0;
	this -> output_path = 0;
	this -> source_text = 0;
	this -> source_length = 0;
	this -> source_tokens = 0;
	this -> tokens_length = 0;
	this -> compiled_text = 0;
	this -> asm_listing = false;
}

Programm::Programm(int argc, char* argv[])
{
	for (int argcounter = 1; argcounter < argc; argcounter++)
	{
		if (!strcmp(argv[argcounter], "-o") or !strcmp(argv[argcounter], "--output"))
			this -> output_path = argv[++argcounter];
		
		else if (!strcmp(argv[argcounter], "-l") or !strcmp(argv[argcounter], "--listing"))
			this -> asm_listing = true;
		
		//else if (!strcmp(argv[argcounter], "-s") or !strcmp(argv[argcounter], "--silent"))
		//	silent
		
		else
			this -> source_path = argv[argcounter];
	}
	
	if (not this -> output_path)
		this -> output_path = "a.out";
		
	this -> source_text = 0;
	this -> source_length = 0;
	this -> source_tokens = 0;
	this -> tokens_length = 0;
	this -> compiled_text = 0;
	if (not this -> asm_listing)
		this -> asm_listing = false;
}


Programm::~Programm()
{
	if (this -> source_text)
		delete this -> source_text;
		
	if (this -> source_tokens)
		delete this -> source_tokens;
		
	if (this -> compiled_text)
		delete this -> compiled_text;
}




int Programm::readSource()
{
	
	FILE* source_file = fopen(this -> source_path, "r");
	
	if (not source_file)
	{
		printf("\x1b[1;31mError\x1b[0m: file %s not exist.\n", this -> source_path);
		exit(FILE_NOT_EXIST);
	}
	
    fseek(source_file, 0, SEEK_END);
    this -> source_length = ftell(source_file);
    rewind(source_file);

    this -> source_text = new char[this -> source_length + 1];
    (this -> source_text)[this -> source_length] = '\0';

    this -> source_length = fread(this -> source_text, sizeof(char), this -> source_length, source_file);
    
    printf("\n\x1b[1;32m%lld\x1b[0;32m symbols readed from \x1b[1;32m%s\x1b[0m\n------------ \x1b[1;32mSOURCE CODE\x1b[0m ------------\n%s\n------------- \x1b[1;32mEND SOURCE\x1b[0m ------------\n\n", this -> source_length, this -> source_path, this -> source_text);
    
	return this -> source_length;
}






// end;
