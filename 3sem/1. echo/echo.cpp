#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace SETTINGS
{
	int keys_count = 0;
	bool new_line_enabled = true;
	bool space_separators_enabled = true;
}


int parseArguments(int argc, char* argv[]);
int giveResponce(int argc, char* argv[]);



int main(int argc, char* argv[])
{
	parseArguments(argc, argv);
	giveResponce(argc, argv);
	return 0;
}



int parseArguments(int argc, char* argv[])
{
	int keys_count = 0;
	
	for (int counter = 1; counter < argc; counter++)
	{		
		if (argv[counter][0] == '-')
		{
			bool is_parameter = false;
			
			if (strchr(argv[counter], 'n'))
			{
				SETTINGS::new_line_enabled = false;
				is_parameter = true;
			}
			
			
			if (strchr(argv[counter], 's'))
			{
				SETTINGS::space_separators_enabled = false;
				is_parameter = true;
			}
			
			if (is_parameter)
			{
				keys_count++;
			}
		}
		
		else
			break;
	}
	
	SETTINGS::keys_count = keys_count;
	
	return 0;
}


int giveResponce(int argc, char* argv[])
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
	
	return 0;
}

