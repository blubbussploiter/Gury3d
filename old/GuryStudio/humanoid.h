#ifndef HUMANOID_H
#define HUMANOID_H

#include "part.h"
#include "snaps.h"

#include "ISteppable.h"

#include <G3DAll.h>

namespace RBX
{
	class Humanoid : public RBX::ISteppable, public RBX::Derivable<RBX::Render::Renderable>
	{
	private:

		enum WalkMode
		{
			DIRECTION_JUMP,
			DIRECTION_MOVE,
		};

		Vector3 walkDirection;
		Vector3 walkRotationVelocity;

		WalkMode walkMode;

		bool jointsBuilt;

	public:

		RBX::PVInstance* humanoidRootPart, * humanoidHead;

		float health;
		float maxHealth;
		float jmpPower;
		bool isDead;

		Humanoid()
		{
			health = 100.0f;
			maxHealth = 100.0f;
			jmpPower = 30;
			setClassName("Humanoid");
			setName("Humanoid");
			isSteppable = 1;
		}

		bool isGrounded();
		bool isFalling();
		bool isInAir();
		bool isJoined();

		bool checkHumanoidAttributes() { return (humanoidHead && humanoidRootPart && (humanoidRootPart->body && humanoidHead->body)); }

		void balance();
		void onDied();
		/* corrects humanoidRootPart and humanoidHead */
		void correctHumanoidAttributes();
		void buildJoints();
		/* get Humanoid from model if its a character (has humanoid) */
		static Humanoid* modelIsCharacter(RBX::Instance* testModel);
		/* sets humanoidRootPart and humanoidHead accordingly */
		void setHumanoidAttributes();
		void setWalkDirection(Vector3 walkDir);
		void setJump();
		void onStep();

		void render(RenderDevice* rd);
		void drawHealthbar(RenderDevice* rd, CoordinateFrame center, float distance);

		RBX::PartInstance* getRightArm();
		RBX::PartInstance* getLeftArm();
		RBX::PartInstance* getRightLeg();
		RBX::PartInstance* getLeftLeg();
		virtual ~Humanoid() {}
	};
}
#endif