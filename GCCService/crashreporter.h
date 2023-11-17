#ifndef CRASHREPORTER_H
#define CRASHREPORTER_H

#include "rbx.h"

namespace RBX
{
	class CrashReporter
	{
	public:
		static CrashReporter* singleton();
		int processException(_EXCEPTION_POINTERS* ExceptionInfo);
		void start();
	};
}

#endif