#ifndef ROCKET_H
#define ROCKET_H

#include "instance.h"
#include "part.h"

namespace RBX
{
	class Rocket : public RBX::Instance
	{
	private:
		RBX::PartInstance* missile, *target;
	public:
		bool active, firedEvent;
		float cartoonFactor, maxSpeed;
		float kTurnP, kTurnD, kThrustP, kThrustD, maxThrust, targetRadius;
		Vector3 targetOffset, maxTorque;
		void step();
		Rocket()
		{
		  active = 0;
		  targetOffset.x = 0.0;
		  targetOffset.y = 0.0;
		  target = 0;
		  targetOffset.z = 0.0;
		  firedEvent = 0;
		  targetRadius = 4.0;
		  maxThrust = 4000.0;
		  kThrustP = 5.0;
		  kThrustD = 0.001;
		  maxSpeed = 30.0;
		  kTurnP = 3000.0;
		  kTurnD = 500.0;
		  maxTorque.x = 400000.0;
		  maxTorque.y = 400000.0;
		  maxTorque.z = 0.0;
		  cartoonFactor = 0.69999999;
		}
	};
}

#endif