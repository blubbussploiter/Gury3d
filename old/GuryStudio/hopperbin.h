#ifndef HOPPERBIN_H
#define HOPPERBIN_H

#include <G3DAll.h>
#include "instance.h"

namespace RBX
{

	enum BinType
	{
		SCRIPT_BIN,
		GAMETOOL_BIN,
		GRAB_BIN,
		CLONE_BIN,
		HAMMER_BIN,
		SLINGSHOT_BIN,
		ROCKET_BIN,
		LASER_BIN
	};

	class HopperBin : public RBX::Instance
	{
	public:
		BinType binType;
		bool active;

		virtual void update(RenderDevice* rd, G3D::UserInput* ui) {}
		virtual void activate() {}
		virtual void deactivate() {}
		HopperBin()
		{
			binType = SCRIPT_BIN;
			active = 0;
			setName("HopperBin");
			setClassName("HopperBin");
		}
		virtual ~HopperBin() { deactivate(); };
	};
}
#endif