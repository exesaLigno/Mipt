/*
 * Task 1 - 13
 * Напишите аналог программы watch, из ключей поддержав только interval.
 * Для определения размера экрана используйте TIOCGWINSZ (man tty_ioctl).
 */

#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>


int interval = 2;


int getKeys(int argc, char* argv[]);


int main(int argc, char* argv[])
{
	int command_start = getKeys(argc, argv);
		
	char* command[argc - command_start + 1];
	command[argc - command_start] = nullptr;
	
	for (int counter = 0; counter < argc - command_start; counter++)
		command[counter] = argv[counter + command_start];
	
	while (true)
	{
		winsize size;
		if (ioctl(0, TIOCGWINSZ, &size) == -1)
		{
			perror("ioctl error");
			exit(1);
		}
		
		pid_t child_id = fork();
		if (child_id == 0)
		{
			execvp(command[0], command);
		}
		
		int status = 0;
		wait(&status);
		
		if (WIFEXITED(status))
			printf("cmd exited with %d\n", WEXITSTATUS(status));
		
		printf("ws_row = %d, ws_col = %d\n", size.ws_row, size.ws_col);
		
		sleep(interval);
	}
	
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
