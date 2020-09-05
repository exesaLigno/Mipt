#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace SETTINGS
{
	int keys_count = 0;
	bool new_line_enabled = true;
	bool space_separators_enabled = true;
}


void parseArguments(int argc, char* argv[]);
void giveResponce(int argc, char* argv[]);



int main(int argc, char* argv[])
{
	parseArguments(argc, argv);
	giveResponce(argc, argv);
	return 0;
}



void parseArguments(int argc, char* argv[])
{	
	for (int counter = 1; counter < argc; counter++)
	{		
		if (argv[counter][0] == '-' and strchr("ns", argv[counter][1]))
		{
			
			if (strchr(argv[counter], 'n'))
				SETTINGS::new_line_enabled = false;
			
			
			if (strchr(argv[counter], 's'))
				SETTINGS::space_separators_enabled = false;

			
			SETTINGS::keys_count += 1;
		}
		
		else
			break;
	}
}


void giveResponce(int argc, char* argv[])
{
	int counter = 1 + SETTINGS::keys_count;
	
	while (counter < argc)
	{
		printf("%s", argv[counter]);
		
		if (SETTINGS::space_separators_enabled == true and counter != argc - 1)
			printf(" ");
		
		counter++;
	}
	
	if (SETTINGS::new_line_enabled == true)
		printf("\n");
}

