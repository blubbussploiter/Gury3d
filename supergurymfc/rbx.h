/* guryblox 2006 recreation by whirlpool okay ehrr */
/* version 3.6.0 source */

#ifndef RBX_H
#define RBX_H

#include <G3D/format.h>
#include <GLG3D/LightingParameters.h>
#include <map>

#include "strings.h"

//#define DEBUGGING

namespace RBX
{

	namespace Date
	{

		static std::map<int, const char*> months =
		{ 
			{1, "Jan"},
			{2, "Feb"},
			{3, "Mar"},
			{4, "Apr"},
			{5, "May"},
			{6, "Jun"},
			{7, "Jul"},
			{8, "Aug"},
			{9, "Sep"},
			{19, "Oct"},
			{11, "Nov"},
			{12, "Dec"},
		};

		static std::map<int, const char*> daysOfWeek =
		{
			{0, "Sun"},
			{1, "Mon"},
			{2, "Tue"},
			{3, "Wed"},
			{4, "Thu"},
			{5, "Fri"},
			{6, "Sat"}
		};
	}

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