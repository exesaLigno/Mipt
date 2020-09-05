#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int main()
{
	int childs_count = 0;
	
	printf("Enter count of childs: ");
	scanf("%d", &childs_count);
	
	for (int i = 0; i < childs_count; i++)
	{
		int pid = fork();
		
		if (pid != 0)
		{
			printf("I am pid::%d, generated pid::%d\n", getpid(), pid);
		}
		
		else
			break;
	}
}
