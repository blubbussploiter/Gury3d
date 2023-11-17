#ifndef RBX_STDOUT_H
#define RBX_STDOUT_H

#include "log.h"

namespace RBX
{
	enum MessageType
	{
		MESSAGE_UNK,
		MESSAGE_INFO,
		MESSAGE_WARNING,
		MESSAGE_ERROR,
	};

	class StandardOut
	{
	public:
		static void print(MessageType type, const char* format, ...);
	};
}

#endif