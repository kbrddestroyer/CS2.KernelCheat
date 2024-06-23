#include "UMLogger.h"

void log_utils::UMLogger::message(char cLablel, std::string sScope, std::string sMessage)
{
#ifdef DEBUG
	std::cout <<
		'[' << cLablel << ']' << ' ' <<
		sScope << sMessage << std::endl;
#endif
}