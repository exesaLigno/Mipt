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
	
	while (counter < childs_count - 1)
	
	printf("Child: %d (created by %d)\n", getpid(), getppid());
}
