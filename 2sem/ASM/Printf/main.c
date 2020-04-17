#include <stdlib.h>
#include <stdio.h>

int forout(const char*, ...);

void beautiful_test();
void clear_test();

int main(int argc, char* argv[])
{
	if (argc == 1)
		beautiful_test();
	else if (!strcmp(argv[1], "--clear"))
		clear_test();
	else if (!strcmp(argv[1], "--beautiful"))
		beautiful_test();
	else
		printf("Unknown parameter!\n");
		
	return 0;
}


void beautiful_test()
{
	char string1[10] = "Vodka";
	char string2[10] = "people";
	int test_number = 0xBEDA;
	int test = 0;

	printf("\x1b[1;32m----- forout test -----\x1b[0m\n\n");
	
	forout("\x1b[5;32m%s connecting %s%c\x1b[0m\n\n", string1, string2, '!');
	forout("\x1b[3m%d\x1b[0m in other cs:\n\t\x1b[3m%b\x1b[0m (binary);\n\t\x1b[3m%q\x1b[0m (quaternary);\n\t\x1b[3m%o\x1b[0m (octal);\n\t\x1b[3m%h\x1b[0m (heximal)\n\n", test_number, test_number, test_number, test_number, test_number);
	forout("Many parameters test: \x1b[3m%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\x1b[0m\n\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	
	printf("\x1b[1;31m----- forout test END -----\x1b[0m\n\n");
}

void clear_test()
{
	char string1[10] = "Vodka";
	char string2[10] = "people";
	int test_number = 0xBEDA;
	int test = 0;

	printf("\x1b[1;32m----- forout test -----\x1b[0m\n\n");
	
	test = forout("%s connecting %s%c\n", string1, string2, '!');
	forout("---Printed %d symbols---\n\n", test);
	
	test = forout("%d in other cs:\n\t%b (binary);\n\t%q (quaternary);\n\t%o (octal);\n\t%h (heximal)\n", test_number, test_number, test_number, test_number, test_number);
	forout("---Printed %d symbols---\n\n", test);
	
	test = forout("Many parameters test: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	forout("---Printed %d symbols---\n\n", test);
	
	printf("\x1b[1;31m----- forout test END -----\x1b[0m\n\n");
}



