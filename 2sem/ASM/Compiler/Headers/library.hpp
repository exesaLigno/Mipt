#pragma once
#include "errcodes.hpp"

#include <fstream>
#include <cstring>


class Library
{
  public:
	char* libname;
	int libname_length;
	char* libtext;
	unsigned long long int libtext_length;
	
	Library();
	~Library();
	int readLibrary();
};