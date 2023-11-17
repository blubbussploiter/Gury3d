#include "log.h"

void RBX::Log::writeEntry(const char* message)
{
	std::string format;

	_SYSTEMTIME SystemTime;
	
	GetLocalTime(&SystemTime);
	format = RBX::Format("%02u:%02u.%03u ", SystemTime.wHour, SystemTime.wMinute, SystemTime.wMilliseconds);

	format += message;
	printf("%s\n", format.c_str());
}
