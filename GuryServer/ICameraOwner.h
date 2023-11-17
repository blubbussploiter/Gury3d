#pragma once

#include "instance.h"

namespace RBX
{
	class ModelInstance;
	class Extents;

	class ICameraOwner /* purely virtual, only used for dynamic_casts */
	{
	public:
		virtual RBX::ModelInstance* getModel() = 0; /* return the actual model that derives from this */
		virtual RBX::Extents computeCameraOwnerExtents() = 0;
	};
}

