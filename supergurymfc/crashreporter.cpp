#include "crashreporter.h"

RBX::CrashReporter* reporter;
bool showedMessage;

RBX::CrashReporter* RBX::CrashReporter::get()
{
	if (!reporter) reporter = new RBX::CrashReporter();
	return reporter;
}

LONG WINAPI CrashExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	return RBX::CrashReporter::get()->processException(ExceptionInfo);
}

int RBX::CrashReporter::processException(_EXCEPTION_POINTERS* ExceptionInfo)
{
	if (!showedMessage)
	{
		showedMessage = 1;
		MessageBoxA(0, "An unexpected error occurred and Gury3D needs to quit.  We're sorry!", "Gury3D Crash", 0);
	}
	return 0;
}

void RBX::CrashReporter::start()
{
	SetUnhandledExceptionFilter(CrashExceptionFilter);
}
