#include "controller.h"
#include "datamodel.h"

#include "stdout.h"

void RBX::Controller::handleInput(G3D::UserInput* ui)
{
	if (isDisabled) return;

	horizontal = ui->getX();
	vertical = ui->getY();

	isMoving = (fabs(horizontal) == 1 || fabs(vertical) == 1);

	if (ui->keyPressed(SDLK_SPACE))
	{
		jumping = true;
	}
}

void RBX::ControllerService::mapControllers(G3D::UserInput*& ui)
{
	Table<UserInput::KeyCode, UserInput::UIFunction> keys;

	keys.set(SDLK_w, UserInput::UP);
	keys.set(SDLK_s, UserInput::DOWN);
	keys.set(SDLK_a, UserInput::LEFT);
	keys.set(SDLK_d, UserInput::RIGHT);

	ui->setKeyMapping(&keys);
}

void RBX::ControllerService::updateControllers(G3D::UserInput* ui)
{
	for (unsigned int i = 0; i < controllers.size(); i++)
	{
		RBX::Controller* c = controllers.at(i);
		if (c && !c->disabled())
		{
			c->handleInput(ui);
			c->move();
			c->mv_update();
		}
	}
}

void RBX::ControllerService::addController(Controller* c)
{
	controllers.push_back(c);
}

RBX::ControllerService* RBX::ControllerService::get()
{
	return RBX::Datamodel::getDatamodel()->controllerService;
}