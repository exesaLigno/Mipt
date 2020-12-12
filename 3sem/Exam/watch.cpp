/*
 * Task 1 - 13
 * Напишите аналог программы watch, из ключей поддержав только interval.
 * Для определения размера экрана используйте TIOCGWINSZ (man tty_ioctl).
 */

#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>

int main()
{
	winsize size;
	if (ioctl(0, TIOCGWINSZ, &size) == -1)
	{
		perror("ioctl error");
		exit(1);
	}
	
	printf("ws_row = %d, ws_col = %d\n", size.ws_row, size.ws_col);
	
    return 0;
}
