#ifndef LOG_H
#define LOG_H

#include "rbx.h"

namespace RBX
{
	class Log
	{
	public:
		static void writeEntry(const char* message);
	};
}

#endif