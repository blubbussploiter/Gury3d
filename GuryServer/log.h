#ifndef LOG_H
#define LOG_H

#include "rbx.h"

namespace RBX
{
	enum MessageType
	{
		MESSAGE_PRINT,
		MESSAGE_INFO,
		MESSAGE_WARNING,
		MESSAGE_ERROR,
	};
	class Log
	{
	public:
		static void cleanup();
		static void writeEntry(MessageType messageType, const char* message);
	};
}

#endif