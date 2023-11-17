#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "controller.h"

namespace RBX
{

	class KeyboardController : public RBX::Controller
	{
	private:
		Instances* motors;
	public:
		void move();

		void mv_update();
		void handleInput(G3D::UserInput* ui);

		void moveCharacter(RBX::Instance* character);
		void bindCharacter(RBX::Instance* character);

		void bindMotors(RBX::Instance* character);

		KeyboardController()
		{
			motors = new Instances();
		}
	};
}

#endif