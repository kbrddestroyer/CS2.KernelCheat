#ifdef DEBUG
#include <iostream>
#endif

#include <httprequestid.h>

void DEBUG_MESSAGE(unsigned short uFlag, const char* sMessage)
{
#ifdef DEBUG
	std::cout << sMessage << std::endl;
#endif
}

