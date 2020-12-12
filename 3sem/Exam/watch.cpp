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
#include <time.h>
#include <cstring>


int interval = 2;


int getKeys(int argc, char* argv[]);
int displayResult(const char* stdout_text, const char* stderr_text, int width, int height, char* command[]);


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
		
		int stdout_pipe_fd[2];
		int stderr_pipe_fd[2];
		pipe(stdout_pipe_fd);
		pipe(stderr_pipe_fd);
		
		pid_t child_id = fork();
		if (child_id == 0)
		{
			close(stdout_pipe_fd[0]);
			close(stderr_pipe_fd[0]);
			
			dup2(stdout_pipe_fd[1], STDOUT_FILENO);
			dup2(stderr_pipe_fd[1], STDERR_FILENO);
			
			close(stdout_pipe_fd[1]);
			close(stderr_pipe_fd[1]);
			
			execvp(command[0], command);
			
			abort();
		}		
		
		int status = 0;
		wait(&status);
		
		if (not WIFEXITED(status))
		{
			printf("command can't be executed, exiting...\n");
			exit(1);
		}
		
		close(stdout_pipe_fd[1]);
		close(stderr_pipe_fd[1]);
		
		int stdout_length = 0;
		int stderr_length = 0;
		ioctl(stdout_pipe_fd[0], FIONREAD, &stdout_length);
		ioctl(stderr_pipe_fd[0], FIONREAD, &stderr_length);
		
		char stdout_text[stdout_length + 1];
		char stderr_text[stderr_length + 1];
		
		read(stdout_pipe_fd[0], stdout_text, stdout_length);
		read(stderr_pipe_fd[0], stderr_text, stderr_length);
		
		close(stdout_pipe_fd[0]);
		close(stderr_pipe_fd[0]);
		
		stdout_text[stdout_length] = '\0';
		stderr_text[stderr_length] = '\0';
		
		displayResult(stdout_text, stderr_text, size.ws_col, size.ws_row, command);
		
		//printf("ws_row = %d, ws_col = %d\n", size.ws_row, size.ws_col);
		
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


int displayResult(const char* stdout_text, const char* stderr_text, int width, int height, char* command[])
{
	int printed_width = 0;
	int printed_height = 0;
	
	printed_width += printf("Every %d second: ", interval);
	int counter = 0;
	while (command[counter] != nullptr)
		printed_width += printf("%s ", command[counter++]);
	
	long int ttime = 0;
	ttime = time(NULL);
	char* time_str = ctime(&ttime);
	int time_len = strlen(time_str);
	
	for (int i = 0; i < width - printed_width - time_len + 1; i++)
		printf(" ");
	
	printf("%s\n", time_str);
	
	printed_height++;
	
	return 0;
}
