#include <stdlib.h>
#include <stdio.h>

int forout(const char*, ...);

int main()
{
	char string1[10] = "Vodka";
	char string2[10] = "people";
	int test_number = 912303;
	int test = forout("%s connecting %s%c\n", string1, string2, '!');
	printf("%d\n", test);
	return 0;
}
