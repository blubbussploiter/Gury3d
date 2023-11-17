#ifndef  CLONEBIN_H
#define CLONEBIN_H

#include "hopperbin.h"
#include "backpack.h"

namespace RBX
{
	class CloneBin : public RBX::HopperBin
	{
	public:
		void update(RenderDevice* rd, G3D::UserInput* ui);
		void deactivate();
		void activate();
		CloneBin()
		{
			binType = CLONE_BIN;
		}

		virtual ~CloneBin(){deactivate();}

	};
}

#endif