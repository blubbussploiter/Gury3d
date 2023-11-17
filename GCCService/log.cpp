
#include "log.h"

std::string dayOfWeek(WORD d)
{
	switch (d)
	{
	case 1:
		return "Monday";
	case 2:
		return "Tuesday";
	case 3:
		return "Wednesday";
	case 4:
		return "Thursday";
	case 5:
		return "Friday";
	case 6:
		return "Saturday";
	case 7:
		return "Sunday";
	}
	return "";
}

void RBX::Log::writeEntry(MessageType messageType, const char* message)
{
	printf("%s", message);
}
