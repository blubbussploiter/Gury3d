#ifndef  ROCKETBIN_H
#define ROCKETBIN_H

#include "hopperbin.h"
#include "rocket.h"

namespace RBX
{
	class RocketBin : public RBX::HopperBin
	{
	private:
		RBX::Rocket* rocket;
	public:
		void update(RenderDevice* rd);
		void deactivate();
		void activate() {};
		RocketBin()
		{
			binType = ROCKET_BIN;
		}
	};
}

#endif
