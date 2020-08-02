#include "../Headers/binary.hpp"


void Binary::optimize()
{
	bool optimized = false;
	
	this -> deleteUnusedFunctions();
	
	if (optimized)
		this -> optimize();
	
	return;
}


void Binary::deleteUnusedFunctions()
{
	
}
