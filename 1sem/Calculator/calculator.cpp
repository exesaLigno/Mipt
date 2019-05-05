#include <fstream>
#include <cstdlib>
#include <cstring>
#include <assert.h>




int     GetG0   (char buffer []);
int     GetE    ();
int     GetT    ();
int     GetP    ();
int     GetN    ();

char* line = 0;
int position = 0;




int main ()
{
    char test [100] = {};//"4+(45*45+34-(9+5)*4)+(((45+3)*4)+4)*89-9";
    scanf ("%s", &test);
    printf ("%s\n", test);
    int result = GetG0 (test);
    printf ("%d\n", result);
    return 0;
}





int GetG0 (char buffer [])
{
    line = buffer;
	position = 0;
	int value = GetE ();
	assert (line [position] == 0);
	return value;
}


int GetE ()
{
    int value = GetT ();
	while (line [position] == '+' || line [position] == '-')
	{
		int op = line [position];
		position++;
		int value2 = GetT ();
		if (op == '+')
            value += value2;
		else if (op == '-')
            value -= value2;
	}
    return value;
}


int GetT ()
{
    int value = GetP ();
	while (line [position] == '*' || line [position] == '/')
	{
		int op = line [position];
		position++;
		int value2 = GetP ();
		if (op == '*')
            value *= value2;
		else if (op == '/')
            value /= value2;
	}
    return value;
}


int GetP ()
{
    if (line [position] == '(')
	{
		position++;
		int value = GetE ();
		assert (line [position] == ')');
		position++;
		return value;
	}
	else
		return GetN ();
}


int GetN ()
{
    int value = 0;
	while ('0' <= line [position] && line [position] <= '9')
	{
		value = value * 10 + line [position] - '0';
		position++;
	}
    return value;
}

//end
