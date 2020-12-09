#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <cstring>
#include <errno.h>


int main()
{
	int ships_count_shm_fd = shm_open("ships_count", O_CREAT | O_RDWR, S_IRWXU);
	ftruncate(ships_count_shm_fd, sizeof(int));
	int* ships_counter = (int*) mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, ships_count_shm_fd, 0);
	
	sem_t* bridge_opened = sem_open("bridge_opened", O_CREAT | O_RDWR);
	sem_t* bridge_closed = sem_open("bridge_closed", O_CREAT | O_RDWR);
	sem_t* cars_moving = sem_open("cars_moving", O_CREAT | O_RDWR);
	sem_t* ships_moving = sem_open("ships_moving", O_CREAT | O_RDWR);
	
	printf("To close bridge press enter..\x1b[5m.\x1b[0m ");
	getchar();
		
	munmap(ships_counter, sizeof(int));
	shm_unlink("ships_count");
	
	sem_close(bridge_opened);
	sem_close(bridge_closed);
	sem_close(cars_moving);
	sem_close(ships_moving);
	
	printf("Exited\n");
	
	return 0;
}
