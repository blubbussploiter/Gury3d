#include "controller.h"
#include "datamodel.h"

void RBX::Controller::addDirection(MovementDirections direction)
{
	if (std::find(directions.begin(),
		directions.end(),
		direction) == directions.end())
	{
		directions.push_back(direction);
	}
}

void RBX::Controller::removeDirection(MovementDirections direction)
{
	directions.erase(std::remove(directions.begin(), directions.end(), direction));
}

void RBX::Controller::handleInput(G3D::UserInput* ui)
{
	if (ui->keyDown(SDLK_w))
	{
		addDirection(Forward);
	}
	if (ui->keyDown(SDLK_s))
	{
		addDirection(Backwards);
	}
	if (ui->keyDown(SDLK_a))
	{
		addDirection(Left);
	}
	if (ui->keyDown(SDLK_d))
	{
		addDirection(Right);
	}
	if (ui->keyReleased(SDLK_SPACE))
	{
		addDirection(Jump);
	}

	if (ui->keyReleased(SDLK_w) ||
		ui->keyReleased(SDLK_a) ||
		ui->keyReleased(SDLK_s) ||
		ui->keyReleased(SDLK_d))
	{
		directions.clear();
		isMoving = false;
	}

	if (ui->keyDown(SDLK_w) ||
		ui->keyDown(SDLK_a) ||
		ui->keyDown(SDLK_s) ||
		ui->keyDown(SDLK_d))
	{
		isMoving = true;
	}
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

RBX::ControllerService* RBX::ControllerService::singleton()
{
	return RBX::Datamodel::getDatamodel()->controllerService;
}