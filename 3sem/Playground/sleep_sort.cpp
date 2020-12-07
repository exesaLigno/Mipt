#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void sleep_sort(int len, int* massive);


int main(int argc, char* argv[])
{	
	int massive[] = {1, 45, 3, 5, 12000, 34, 32, 5, 12, 9, 89};
	int len = 11;
	
	sleep_sort(len, massive);
}


void sleep_sort(int len, int* massive)
{
	int fuck = 0;
	
	
	for (int counter = 0; counter < len; counter++)
	{
		pid_t created = fork();
		
		if (created == 0)
		{
			usleep(massive[counter] * 500);
			printf("%d ", massive[counter]);
			exit(0);
		}
	}
	
	while (wait(&fuck) != -1);
	
	printf("\n");
}
