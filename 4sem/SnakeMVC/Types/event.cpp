#include "event.hpp"


Event::Event(int event_type)
{
	this -> event_code = event_type;
}


bool Event::operator==(int event_type)
{
	if (this -> event_code == event_type)
		return true;
	
	return false;
}
