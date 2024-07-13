#pragma once

#include "instance.h"

namespace RBX
{
	class ISteppable
	{
	public:
		virtual void onStep() = 0;
		virtual void onKernelStep() = 0;
	};
}

