#include <stdlib.h>
#include <stdio.h>

int forout(const char*, ...);

int main()
{
	char string1[10] = "Vodka";
	char string2[10] = "people";
	int test_number = 0xBEDA;
	int test = forout("%s connecting %s%c\n", string1, string2, '!');
	forout("---Printed %d symbols---\n\n", test);
	test = forout("%d in other cs:\n\t%b (binary);\n\t%q (quaternary);\n\t%o (octal);\n\t%h (heximal)\n", test_number, test_number, test_number, test_number, test_number);
	forout("---Printed %d symbols---\n\n", test);
	forout("Many parameters test: %d %d %d %d %d %d %d %d %d %d %d %d\n\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	return 0;
}
