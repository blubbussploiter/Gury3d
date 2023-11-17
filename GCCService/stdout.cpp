#include "stdout.h"

void RBX::StandardOut::print(MessageType type, const char* format, ...)
{
	va_list argPtr;
	std::string _fmt, date;

	_SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	va_start(argPtr, format);
	_fmt = G3D::vformat(format, argPtr);

	date = RBX::Format("%d", SystemTime.wDay);

	//RBX::Log::writeEntry(MESSAGE_PRINT, date.c_str());

	RBX::Log::writeEntry(type, _fmt.c_str());
	RBX::Log::writeEntry(type, "\n");

	va_end(argPtr);
}