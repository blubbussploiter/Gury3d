
#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <vector>

#include "rttr/registration.h"

#pragma comment(lib, "librttr_core.lib")

namespace RBX
{
	template<class Derived>
	class Derivable :
		public Derived
	{
		RTTR_ENABLE(Derived)
	};
}

#endif