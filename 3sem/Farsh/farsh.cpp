#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <pwd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

//#define DEBUG


const int OK = 0;
const int EXIT = 1;


class Shell
{
  private:
	char* current_directory_str = nullptr;
	char* current_user_str = nullptr;
	char* current_pc_name = nullptr;
	int last_status = 0;

  public:
	Shell();
	~Shell();
	void printGreeting();
	int executeCommand(char* command);
	void polling();
};



Shell::Shell()
{
	this -> current_directory_str = new char[1000];
	getcwd(this -> current_directory_str, 1000);
	this -> current_user_str = new char[100];
	strcpy(this -> current_user_str, getpwuid(getuid()) -> pw_name);
	this -> current_pc_name = new char[100];
	gethostname(this -> current_pc_name, 100);
}

Shell::~Shell()
{
	if (this -> current_directory_str)
		delete[] this -> current_directory_str;
}

void Shell::printGreeting()
{
	char greeting[1000] = {};
	char* full_path = this -> current_directory_str;

	char home_path[1000] = {};
	strcat(home_path, "/home/");
	strcat(home_path, this -> current_user_str);

	if (!strncmp(this -> current_directory_str, home_path, strlen(home_path)))
	{
		strcat(greeting, "~");
		full_path += + strlen(home_path);
	}
	
	while (true)
	{
		if (*full_path == '/')
		{
			strcat(greeting, "/");
			full_path++;
		}

		char* next_slash = strchr(full_path, '/');

		if (next_slash != nullptr)
		{
			strncat(greeting, full_path, 1);
			full_path = next_slash;
		}

		else
		{
			strcat(greeting, full_path);
			break;
		}
	}
	
	printf("\x1b[34m%s\x1b[0m@%s \x1b[32m%s\x1b[0m", this -> current_user_str, this -> current_pc_name, greeting);
	
	if (this -> last_status)
		printf(" \x1b[31m[%d]\x1b[0m", this -> last_status);
	
	printf("> ");
}


void Shell::polling()
{
	printf("\x1b[3mWelcome to \x1b[3;32mfarsh\x1b[0m\x1b[3m, Futile Awful Retard Shell!\n\n");
	
	while (true)
	{
		this -> printGreeting();
		char command[1000] = {};
		
		int counter = 0;
		char symbol = 0;
		
		while (true)
		{
			symbol = getchar();
			
			if (symbol == '\n')
				break;
			
			else if (symbol == EOF)
			{
				printf("exiting...\n");
				return;
			}
			
			else
				command[counter++] = symbol;
		}
		
		int status = this -> executeCommand(command);
		
		if (status == EXIT)
			return;
	}
}


int Shell::executeCommand(char* command)
{	
	int word_count = 0;
	
	int counter = 0;
	
	while (command[counter] != '\0')
	{
		while (command[counter] == ' ')
			counter++;
		
		if (command[counter] != '\0' and command[counter] != ' ')
			word_count++;
		
		if (command[counter] == '\"')
		{
			counter++;
			
			while (command[counter] != '\"')
				counter++;
			
			counter++;
		}
		
		else if (command[counter] == '\'')
		{
			counter++;
			
			while (command[counter] != '\'')
				counter++;
			
			counter++;
		}
		
		else
		{
			while (command[counter] != ' ' and command[counter] != '\0')
				counter++;
		}
	}
		
	char* command_parsed[word_count + 1];
	
	if (word_count == 0)
		return OK;
	
	word_count = 0;
	
	counter = 0;
	
	while (command[counter] != '\0')
	{
		while (command[counter] == ' ')
		{
			command[counter] = '\0';
			counter++;
		}
		
		if (command[counter] != '\0' and command[counter] != ' ')
		{
			command_parsed[word_count] = command + counter;
			
			if (command[counter] == '\"' or command[counter] == '\'')
				command_parsed[word_count]++;
			
			word_count++;
		}
		
		if (command[counter] == '\"')
		{
			command[counter++] = '\0';
			
			while (command[counter] != '\"')
				counter++;
			
			command[counter++] = '\0';
		}
		
		else if (command[counter] == '\'')
		{
			command[counter++] = '\0';
						
			while (command[counter] != '\'')
				counter++;
			
			command[counter++] = '\0';
		}
		
		else
		{
			while (command[counter] != ' ' and command[counter] != '\0')
				counter++;
		}
	}
	
	command_parsed[word_count] = NULL;
	
	if (!strcmp(command_parsed[0], "exit"))
		return EXIT;
	
	if (!strcmp(command_parsed[0], "cd") and word_count > 1)
	{
		chdir(command_parsed[1]);
		getcwd(this -> current_directory_str, 1000);
		return OK;
	}
	
	counter = 0;
	int commands_count = 1;
	
	while (command_parsed[counter] != NULL)
	{
		if (!strcmp(command_parsed[counter], "|"))
			commands_count++;
		
		counter++;
	}
	
	
	char** command_set[commands_count];
	int commands_counter = 0;
	counter = 0;
	
	while (command_parsed[counter] != NULL)
	{
		if (strcmp(command_parsed[counter], "|"))
		{
			command_set[commands_counter] = command_parsed + counter;
			
			commands_counter++;
			
			while (command_parsed[counter] != NULL and strcmp(command_parsed[counter], "|"))
				counter++;
		}
		
		if (command_parsed[counter] != NULL and !strcmp(command_parsed[counter], "|"))
		{
			command_parsed[counter] = NULL;
			counter++;
		}
	}
	
	
	pid_t child = fork();
	
	int wstatus = 0;
	
	if (!child)
	{
		int counter = 0;
		
		while (true)
		{
			if (counter < commands_count - 1)
			{
				int pipe_fd[2];
				pipe(pipe_fd);
				
				int result = fork();
				
				if (result != 0)
				{
					close(pipe_fd[1]);
					
					dup2(pipe_fd[0], STDIN_FILENO);
					close(pipe_fd[0]);
					
					break;
				}
				
				else
				{
					close(pipe_fd[0]);
					
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[1]);
					
					counter++;
				}
			}
			
			else
			{
				break;
			}
		}
		
				execvp(command_set[commands_count - 1 - counter][0], command_set[commands_count - 1 - counter]);
		
		#ifdef DEBUG
			perror("something gone wrong");
		#endif
		
		abort();		// TODO replace magic number with any correct way to catch unknown commands
	}
	
	else
	{
		wait(&wstatus);
		
		if (not WIFEXITED(wstatus))
			printf("farsh: \x1b[1;31mUnknown command\x1b[0m: %s\n", command);
		
		else
			this -> last_status = WEXITSTATUS(wstatus);
	}
	
	return OK;
}


int main()
{
	Shell farSH;
	farSH.polling();
	return 0;
}
