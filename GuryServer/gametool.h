#ifndef GAMETOOL_H
#define GAMETOOL_H

#include "hopperbin.h"
#include "pvinstance.h"

namespace RBX
{
	class GameTool : public RBX::HopperBin
	{
	private:
		RBX::PVInstance* dragged;
		Vector3 lastPosition;
	public:
		void update(RenderDevice* rd, G3D::UserInput* ui);
		void deactivate();
		void activate();
		GameTool()
		{
			binType = GAMETOOL_BIN;
			dragged = 0;
		}
	};
}

#endif