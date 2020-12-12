/*
 * Task 1 - 13
 * Напишите аналог программы watch, из ключей поддержав только interval.
 * Для определения размера экрана используйте TIOCGWINSZ (man tty_ioctl).
 */

#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>


int interval = 2;


int getKeys(int argc, char* argv[]);


int main(int argc, char* argv[])
{
	int command_start = getKeys(argc, argv);
	
	printf("command starts on %d parameter\n", command_start);
	
	winsize size;
	if (ioctl(0, TIOCGWINSZ, &size) == -1)
	{
		perror("ioctl error");
		exit(1);
	}
	
	printf("ws_row = %d, ws_col = %d\n", size.ws_row, size.ws_col);
	
    return 0;
}


int getKeys(int argc, char* argv[])
{
	int counter = 0;
	
	for (counter = 1; counter < argc; counter++)
	{
		if (argv[counter][0] == '-')
		{
			if (argv[counter][1] == 'n')
				interval = atoi(argv[++counter]);
		}
		
		else
			break;
	}
	
	return counter;
}
