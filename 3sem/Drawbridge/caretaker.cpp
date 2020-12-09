/* 
 * This is caretaker utility
 * It will open all shm's and semaphores
 * After exiting it will close all shared memory
 */


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
	
	sem_t* bridge_opened = sem_open("bridge_opened", O_CREAT | O_RDWR, S_IRWXU, 1);
	if (bridge_opened == SEM_FAILED)
		perror("bridge_opened sem_open()");
	
	sem_t* bridge_closed = sem_open("bridge_closed", O_CREAT | O_RDWR, S_IRWXU, 1);
	if (bridge_closed == SEM_FAILED)
		perror("bridge_closed sem_open()");
	
	sem_t* cars_moving = sem_open("cars_moving", O_CREAT | O_RDWR, S_IRWXU, 1);
	if (cars_moving == SEM_FAILED)
		perror("cars_moving sem_open()");
	
	sem_t* ships_moving = sem_open("ships_moving", O_CREAT | O_RDWR, S_IRWXU, 1);
	if (ships_moving == SEM_FAILED)
		perror("ships_moving sem_open()");
	
	printf("To close bridge press enter..\x1b[5m.\x1b[0m ");
	getchar();
		
	munmap(ships_counter, sizeof(int));
	shm_unlink("ships_count");
	
	sem_close(bridge_opened);
	sem_unlink("bridge_opened");
	
	sem_close(bridge_closed);
	sem_unlink("bridge_closed");
	
	sem_close(cars_moving);
	sem_unlink("cars_moving");
	
	sem_close(ships_moving);
	sem_unlink("ships_moving");
	
	printf("Exited\n");
	
	return 0;
}
