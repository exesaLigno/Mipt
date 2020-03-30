#include <stdlib.h>
#include <stdio.h>

void* my_memset(void* ptr, int value, size_t num);
void* my_memcpy(void* destination, void* source, size_t num);
void* my_memchr(void* ptr, int value, size_t num);
int my_memcmp(void* ptr1, void* ptr2, size_t num);

int my_strlen(char* str);
char* my_strchr(char* str, int character);
char* my_strrchr(char* str, int character);
char* my_strcpy(char* destination, char* source);
char* my_strstr(char* str1, char* str2);

int main()
{
	char test1 [20] = "vodka - pivo";
	char test2 [20] = "pivasik";
	printf("test1 = \"%s\"\ntest2 = \"%s\"\n\n", test1, test2);
	
	printf("my_strlen test:\n");
	printf("test1 length = %d\n\n", my_strlen(test1));
	
	printf("my_strchr test:\n");
	char* strchr_result = my_strchr(test1, 'v');
	printf("first entry of symbol \'v\' in test1 finded at %p (%ld's symbol)\n\n", strchr_result, strchr_result - test1);
	
	printf("my_strrchr test:\n");
	char* strrchr_result = my_strrchr(test1, 'v');
	printf("last entry of symbol \'v\' in test1 finded at %p (%ld's symbol)\n\n", strrchr_result, strrchr_result - test1);
	
	return 0;
}
