#pragma once

#include "instance.h"
#include "datamodel.h"

namespace RBX
{
	template <typename T>
	class Service
	{
	public:
		Service()
		{
			((T*)(this))->setParent(Datamodel::getDatamodel());
			((T*)(this))->isParentLocked = 1;
		}
	};
}