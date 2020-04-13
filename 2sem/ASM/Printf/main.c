#include <stdlib.h>
#include <stdio.h>

int forout_old(const char*, ...);
int forout(const char*, ...);

int main()
{
	char string1[10] = "Vodka";
	char string2[10] = "people";
	int test_number = 0xBEDA;
	int test = 0;
	
	printf("\x1b[1;32m----- Old forout test -----\x1b[0m\n\n");
	test = forout_nomem("\x1b[1;32m%s\x1b[0m connecting %s%c\n", string1, string2, '!');
	forout_nomem("---Printed %d symbols---\n\n", test);
	test = forout_nomem("%d in other cs:\n\t%b (binary);\n\t%q (quaternary);\n\t%o (octal);\n\t%h (heximal)\n", test_number, test_number, test_number, test_number, test_number);
	forout_nomem("---Printed %d symbols---\n\n", test);
	forout_nomem("Many parameters test: %d %d %d %d %d %d %d %d %d %d %d %d\n\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	printf("\x1b[1;31m----- Old forout test END -----\x1b[0m\n\n");

	printf("\x1b[1;32m----- NEW forout test -----\x1b[0m\n\n");
	test = forout("\x1b[1;32m%s\x1b[0m connecting %s%c\n", string1, string2, '!');
	forout("---Printed %d symbols---\n\n", test);
	test = forout("%d in other cs:\n\t%b (binary);\n\t%q (quaternary);\n\t%o (octal);\n\t%h (heximal)\n", test_number, test_number, test_number, test_number, test_number);
	forout("---Printed %d symbols---\n\n", test);
	forout("Many parameters test: %d %d %d %d %d %d %d %d %d %d %d %d\n\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	printf("\x1b[1;31m----- NEW forout test END -----\x1b[0m\n\n");
	
	return 0;
}
