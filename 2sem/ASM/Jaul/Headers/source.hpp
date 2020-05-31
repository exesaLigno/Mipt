class Source
{
  protected:
	char* name;
	short int source_type;
	char* text;
	long long int text_length;
	
  public:
	Source(const char* name, short int source_type);
	~Source();
	
	int open();
	Source* getDefinitions();
	int substitute(Source& source);
	
	enum SourceTypes
	{
		JAUL_SOURCE,
		NASM_SOURCE,
		OBJ_SOURCE,
		DEFINITION
	};
	
	enum Errors
	{
		OPENNING_PROHIBITED = -1,
		FILE_NOT_EXIST = -2,
		UNSUPPORTED_FILE_EXTENSION = -3
	};
}
