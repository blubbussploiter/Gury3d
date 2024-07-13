#ifndef HUMANOID_H
#define HUMANOID_H

#include "part.h"

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
		Tripped,
		Landed
	};
	/* ajajajsjjs */

	struct groundData
	{
		float distanceFrom;
		Vector3 hit;
		Vector3 normal;
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

		WalkMode walkMode;
		HumanoidStates humanoidState;

		bool wasTripped;

		bool jointsBuilt;
		bool attemptingToBalance;
		bool currentlyJumping;
		bool canJump;

		float genieHeight;
		float jumpClock, jumpTimer;

	public:

		RBX::PVInstance* humanoidRootPart, * humanoidHead, *rightLeg, *leftLeg, *rightArm, *leftArm;

		float health;
		float maxHealth;

		float r_turnVelocity;

		bool isDead;
		bool isClimbing;
		bool jumping;

		Humanoid()
		{
			health = 100.0f;
			maxHealth = 100.0f;
			jumpTimer = 1.5f;
			jumpClock = 0.0f;
			jumping = 0;
			genieHeight = 3.9f;
			canJump = 1;
			currentlyJumping = 0;
			renderedLast = 1;
			setClassName("Humanoid");
			setName("Humanoid");
		}

		bool limbsCheck(); /* double check checkHumanoidAttributes, returns result of second check, if not second check, return false indefinitely (until limbs appear) */

		bool checkHumanoidAttributes();
		void snap(PVInstance* p0, PVInstance* p1);

		bool isFalling();
		bool isGrounded();
		bool isJoined();

		void setLegCollisions(bool collidable);
		void setArmCollisions(bool collidable);

		void adjustLimbPhysics();
		void buildJoints();

		void setLocalTransparency(float transparency);

		/* sets humanoidRootPart and humanoidHead accordingly */

		void setHumanoidAttributes();

		void setWalkDirection(Vector3 walkDir);
		void setJump(bool jump=1);

		void onStep();
		void onKernelStep();

		void onDied();
		void onStrafe();
		void onTurn();
		void onMovement();
		void onJump();

		void doSounds();

		void jumpTimeStep();
		void resetJumpTimer();

		void updateHumanoidState();

		void render(RenderDevice* rd);
		void drawHealthbar(RenderDevice* rd, CoordinateFrame center, float distance);
		
		groundData* getHumanoidGroundData();

		static Humanoid* modelIsCharacter(RBX::Instance* testModel);

		/* balancing physics stuff */

		void getFeetOffGround(float damper=0.2f, float multiplier=2.f); /* balancing */
		void genieFloat();

		void tryEnable();

		RBX::PVInstance* getRightArm();
		RBX::PVInstance* getLeftArm();
		RBX::PVInstance* getRightLeg();
		RBX::PVInstance* getLeftLeg();

		virtual ~Humanoid() {}

		RTTR_ENABLE(RBX::Render::Renderable);
	};
}
#endif