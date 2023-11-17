#ifndef HUMANOID_H
#define HUMANOID_H

#include "part.h"
#include "snaps.h"

#include "ISteppable.h"

#include <G3DAll.h>

namespace RBX
{

	enum HumanoidStates
	{
		Running,
		Strafing,
		Jumping,
		Falling,
		Landed
	};

	class Humanoid : public RBX::ISteppable, public RBX::Render::Renderable
	{
	private:

		enum WalkMode
		{
			DIRECTION_JUMP,
			DIRECTION_MOVE,
			DIRECTION_STAY
		};

		Vector3 walkDirection;
		Vector3 walkRotationVelocity;

		WalkMode walkMode;
		HumanoidStates humanoidState;

		bool jointsBuilt;
		float elapsedClimb, climbHeight;

	public:

		RBX::PVInstance* humanoidRootPart, * humanoidHead, *rightLeg, *leftLeg, *rightArm, *leftArm;

		float health;
		float maxHealth;

		bool isDead;
		bool isJumping;
		bool isClimbing;

		Humanoid()
		{
			health = 100.0f;
			maxHealth = 100.0f;
			setClassName("Humanoid");
			setName("Humanoid");
			isSteppable = 1;
			renderedLast = 1;
		}

		bool isGrounded();
		bool isFalling();
		bool isInAir();
		bool isJoined();

		bool checkHumanoidAttributes() { return (humanoidHead && humanoidRootPart && (humanoidRootPart->body && humanoidHead->body)); }

		void climb();

		void onDied();
		void computeFallLumbedo(); /* lumbedo means nothjing i just made it up cuz it sounds cool */

		/* corrects humanoidRootPart and humanoidHead */
		void correctHumanoidAttributes();
		void buildJoints();
		/* get Humanoid from model if its a character (has humanoid) */
		static Humanoid* modelIsCharacter(RBX::Instance* testModel);
		/* sets humanoidRootPart and humanoidHead accordingly */
		void setLocalTransparency(float transparency);
		void setHumanoidAttributes();
		void setWalkDirection(Vector3 walkDir);
		void setJump(bool jump=1);

		void turn();

		void onStep();

		void updateHumanoidState();

		void render(RenderDevice* rd);
		void drawHealthbar(RenderDevice* rd, CoordinateFrame center, float distance);

		void snap(PVInstance* p0, PVInstance* p1);

		RBX::PVInstance* getRightArm();
		RBX::PVInstance* getLeftArm();
		RBX::PVInstance* getRightLeg();
		RBX::PVInstance* getLeftLeg();

		virtual ~Humanoid() {}

		RTTR_ENABLE(RBX::Render::Renderable);
	};
}
#endif