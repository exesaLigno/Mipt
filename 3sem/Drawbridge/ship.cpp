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
#include <cstring>
#include <errno.h>


int main()
{
	int ships_count_shm_fd = shm_open("ships_count", O_RDWR, S_IRWXU);
	ftruncate(ships_count_shm_fd, sizeof(int));
	int* ships_counter = (int*) mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, ships_count_shm_fd, 0);
	
	//sem_t* bridge_opened = sem_open("bridge_opened", O_RDWR);
	//sem_t* bridge_closed = sem_open("bridge_closed", O_RDWR);
	//sem_t* cars_moving = sem_open("cars_moving", O_RDWR);
	//sem_t* ships_moving = sem_open("ships_moving", O_RDWR);
	
	*ships_counter += 1;
	printf("\x1b[1;32mship\x1b[0m> I arrived to the bridge\n");
	
	printf("\x1b[1;32mship\x1b[0m> Passing the bridge\n");
	
	printf("\x1b[1;32mship\x1b[0m> Passed the bridge! Buy-buy!\n");
	*ships_counter -= 1;
	
	return 0;
}
