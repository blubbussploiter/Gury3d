#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <G3DAll.h>

#include "instance.h"
#include "service.h"

namespace RBX
{
	enum MovementDirections
	{
		Idle,
		Forward,
		Backwards,
		Left,
		Right,
		Jump
	};

	enum ControllerTypes
	{
		None,
		KeyboardLeft,
		KeyboardRight
	};

	class Controller : 
		public RBX::Instance
	{
	private:
		bool isMoving;
		bool isDisabled;
		float speed;
	public:

		std::vector<MovementDirections> directions;

		void addDirection(MovementDirections direction);
		void removeDirection(MovementDirections direction);

		float getSpeed() { return speed; }
		void setSpeed(float s) { speed = s; }

		bool moving() { return isMoving; }

		void setmoving(bool m) { isMoving = m; }
		void setdir(MovementDirections d) { directions.push_back(d); }

		void disable(bool d) { isDisabled = d; }
		bool disabled() { return isDisabled; }

		virtual void handleInput(G3D::UserInput* ui);
		virtual void move() {};

		virtual void mv_update() {}

		Controller() : speed(0.2f)
		{
			isMoving = 0;
			isDisabled = 0;
		}
		RTTR_ENABLE(RBX::Instance)
	};

	class ControllerService :
		public RBX::Instance,
		public RBX::Service<ControllerService>
	{
	private:
		std::vector<Controller*> controllers;
	public:
		void updateControllers(G3D::UserInput* ui);
		void addController(Controller* c);
		static ControllerService* singleton();
		ControllerService()
		{
			setClassName("ControllerService");
			setName("ControllerService");
		}
	};

}
#endif