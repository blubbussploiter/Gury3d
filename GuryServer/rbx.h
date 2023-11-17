/* guryblox 2006 recreation by whirlpool okay ehrr */
/* version 3.2.2 source */

#ifndef RBX_H
#define RBX_H

#include <G3DAll.h>
#include "strings.h"

//#define DEBUGGING

namespace RBX
{

	static std::string Format(std::string fmt, ...)
	{
		va_list argPtr;
		std::string _fmt;

		va_start(argPtr, fmt);
		_fmt = G3D::vformat(fmt.c_str(), argPtr);
		va_end(argPtr);

		return _fmt;
	}

}

#endif