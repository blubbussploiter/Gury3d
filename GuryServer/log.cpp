
#include <fstream>

#include "log.h"
#include "stdout.h"

std::ofstream* log_outStream;

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

void RBX::Log::cleanup()
{
	if (log_outStream)
	{
		log_outStream->close();
	}
}

void RBX::Log::writeEntry(MessageType messageType, const char* message)
{
	printf("%s", message);

	if (!log_outStream)
	{
		log_outStream = new std::ofstream(ConFileInPath("\\gury-log.txt"));
	}
	else
	{
		if (log_outStream->is_open())
		{
			log_outStream->write(message, strlen(message));
			log_outStream->flush();
		}
		else
		{
			RBX::StandardOut::print(RBX::MESSAGE_ERROR, "Cannot write to log file");
		}
	}
}
