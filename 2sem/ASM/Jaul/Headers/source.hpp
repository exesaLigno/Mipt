class Source
{
  private:
	char* name;
	short int source_type;
	char* text;
	long long int text_length;
	
  public:
	Source(const char* name);
	~Source();
	
	int open();
	Source* getDefinitions();
	int substitute(Source& source);
}
