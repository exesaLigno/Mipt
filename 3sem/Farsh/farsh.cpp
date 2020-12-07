#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <pwd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>


class Shell
{
  private:
	char* current_directory_str = nullptr;
	char* current_user_str = nullptr;
	char* current_pc_name = nullptr;

  public:
	Shell();
	~Shell();
	void printGreeting();
	void executeCommand(char* command);
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

	//strcat(greeting, full_path);

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


	printf("\x1b[34m%s\x1b[0m@%s \x1b[32m%s\x1b[0m> ", this -> current_user_str, this -> current_pc_name, greeting);

	//printf("\x1b[33m%s\x1b[0m \x1b[32m%s\x1b[0m\n", this -> current_user_str, this -> current_directory_str);
}


void Shell::polling()
{
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

// 			TODO Arrows buttons processing is not implemented yet
// 			else if (symbol == 27)
// 			{
// 				getchar();
// 				char cmd = getchar();
// 				//printf("%d\n", cmd);
// 				//printf("\b\b\b");
// 			}

			else
				command[counter++] = symbol;
		}

		this -> executeCommand(command);
	}
}


void Shell::executeCommand(char* command)
{
	int word_count = 0;
	
	int counter = 0;
	
	while (command[counter] != '\0')
	{
		while (command[counter] == ' ')
			counter++;
		
		if (command[counter] != '\0' and command[counter] != ' ')
			word_count++;
		
		while (command[counter] != ' ' and command[counter] != '\0')
			counter++;
	}
	
	char* command_parsed[word_count + 1];
		
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
			word_count++;
		}
		
		while (command[counter] != ' ' and command[counter] != '\0')
			counter++;
	}
	
	command_parsed[word_count] = NULL;
	
	pid_t child = fork();
	
	int wstatus = 0;
	
	if (!child)
	{
		execvp(command_parsed[0], command_parsed);
		perror("something gone wrong");
		exit(1);
	}
	
	else
	{
		wait(&wstatus);
		if (wstatus != 0)
			printf("can't execute command «%s»\n", command);
			
	}
}


int main()
{
	Shell farSH;
	farSH.polling();
	return 0;
}
