#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int childs_count = 0;
	printf("Childs count: ");
	scanf("%d", &childs_count);
	
	printf("Parent: %d\n", getpid());
	
	for (int counter = 0; counter < childs_count; counter++)
	{
		pid_t created = fork();
		
		printf("return of fork = %d\n\n", created);
		
		int test = 0;
		
		if (created == 0)
		{
			printf("Child: %d (created by %d)\n", getpid(), getppid());
			break;
		}
		
		else
		{
			wait(&test);
		}
	}
	
	return 0;
}
