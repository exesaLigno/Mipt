#include <cstdio>
#include <cstring>


const int DEFAULT_STRING_LENGTH = 1000;


bool line_numering = false;
int line_number = 1;
bool new_line = true;


int parseKeys(int argc, char* argv[]);
int getFile(const char* filename);
int getStdin();
int print(const char* string, int length);


int main(int argc, char* argv[])
{
	parseKeys(argc, argv);
	
	if (argc == 1)
	{
		getStdin();
	}
	
	for (int file_counter = 1; file_counter < argc; file_counter++)
	{
		if (argv[file_counter][0] == '-' and argv[file_counter][1] != '\0')
			continue;
		
		else if (argv[file_counter][0] == '-')
			getStdin();
		
		else		
			getFile(argv[file_counter]);
	}
	
	return 0;
}


int parseKeys(int argc, char* argv[])
{
	for (int counter = 1; counter < argc; counter++)
	{
		if (argv[counter][0] == '-')
		{
			if (argv[counter][1] == 'n')
				line_numering = true;
		}
	}
	
	return 0;
}


int getFile(const char* filename)
{
	FILE* file = fopen(filename, "r");
	fseek(file, 0, SEEK_END);
	long unsigned int length = ftell(file);
	rewind(file);
	
	char file_text[length];
	
	fread(file_text, sizeof(char), length, file);
	
	print(file_text, length);
	
	return 0;
}


int getStdin()
{	
	char* std_input = new char[DEFAULT_STRING_LENGTH]{0};
	
	int counter = 0;
	while ((std_input[counter] = getchar()) != EOF and counter < DEFAULT_STRING_LENGTH)
		counter++;
		
	print(std_input, counter);
	
	delete[] std_input;
	
	return 0;
}


int print(const char* string, int length)
{
	for (int counter = 0; counter < length; counter++)
	{
		if (line_numering and new_line)
			printf("%6d  ", line_number);
		
		putchar(string[counter]);
		new_line = false;
		
		if (string[counter] == '\n')
		{
			line_number++;
			new_line = true;
		}
	}
	
	return 0;
}
