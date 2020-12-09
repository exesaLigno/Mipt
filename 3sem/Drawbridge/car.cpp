/*
 * Ну что нибудь тип такого: у моста три семафора: на состояние разведено, 
 * на состояние сведено и состояние корабли идут и разделяемая переменная с 
 * количеством кораблей перед мостом. Когда корабль появляется он добавляет 
 * к этой переменной единицу. Условие для машины на взятие семафора для машины 
 * это не взят семафор корабли плывут и мост разведён и кораблей меньше двух. 
 * Тогда она берет семафор на машину, проезжает и отдаёт семафор. Корабль 
 * когда походит он проверяет семафор на мост сведён, если он не взят и 
 * если кораблей больше двух, то он берет семафор мост разведён и берет 
 * семафор корабли плывут. Когда он проплыл он уменьшает переменную на 1 
 * и отпускает семафор мост разведён. Следующий корабль берет мост разведён, 
 * проплывает вычитает 1, отдаёт семафор и если в переменной 0, то отдаёт 
 * семафор корабли плывут. Если не 0, то следующий корабль делает то же самое
 */


#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/sem.h>


int main()
{
	int ships_count_shm_fd = shm_open("ships_count", O_RDWR, S_IRWXU);
	ftruncate(ships_count_shm_fd, sizeof(int));
	int* ships_counter = (int*) mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, ships_count_shm_fd, 0);
	
	sem_t* cars_moving = sem_open("cars_moving", O_RDWR);
	sem_t* ships_moving = sem_open("ships_moving", O_RDWR);
	
	printf("\x1b[1;33mcar_%d\x1b[0m> I arrived to the bridge\n", getpid());
	
	sem_wait(cars_moving);	 	// Ждем пока машина уедет с моста
	
	if (*ships_counter >= 2)
	{
		sem_post(ships_moving);
	
		while (*ships_counter > 0)
		{
			sem_wait(cars_moving);
		}
	}
	
	printf("\x1b[1;33mcar_%d\x1b[0m> Passing the bridge\n", getpid());
	sleep(1); 					// Машина проезжает мост за одну секунду
	
	printf("\x1b[1;33mcar_%d\x1b[0m> Passed the bridge! Buy-buy!\n", getpid());
	
	sem_post(cars_moving);		// разблокирует въезд для остальных когда машина уезжает с моста
	
	return 0;
}
