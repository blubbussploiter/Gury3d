#include "stdout.h"

void RBX::StandardOut::print(MessageType type, const char* format, ...)
{
	va_list argPtr;
	std::string _fmt;

	va_start(argPtr, format);
	_fmt = G3D::vformat(format, argPtr);

	RBX::Log::writeEntry(_fmt.c_str());

	va_end(argPtr);
}