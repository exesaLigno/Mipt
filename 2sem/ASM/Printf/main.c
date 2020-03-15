#include <stdlib.h>
#include <stdio.h>

int forout(const char*, ...);

int main()
{
	char string1[10] = "Vodka";
	char string2[10] = "people";
	int test_number = 912303;
	int test1 = forout("%s connecting %s%c\n", string1, string2, '!');
	int test2 = forout("%d in other cs:\n %b (binary);\n %q (quatro)\n %o (octal);\n %h (heximal)\n", test_number, test_number, test_number, test_number, test_number);
	printf("%d %d\n", test1, test2);
	return 0;
}
