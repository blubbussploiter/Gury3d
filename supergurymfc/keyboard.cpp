#include "keyboard.h"
#include "model.h"

void RBX::KeyboardController::moveCharacter(RBX::Instance* character)
{
	RBX::ModelInstance* characterModel;
	RBX::PVInstance* primaryPart;

	btRigidBody* body;

	if (!moving()) return;

	if (character->getClassName() != "Model")
	{
		return;
	}

	characterModel = (RBX::ModelInstance*)(character);
	primaryPart = characterModel->getPrimaryPart();

	body = primaryPart->body->_body;
	if (!body) return;

	switch (dir())
	{
		case Forward:
		{
			
			break;
		}
	}

}

void RBX::KeyboardController::bindCharacter(RBX::Instance* character)
{
	RBX::ModelInstance* characterModel;
	if (character->getClassName() != "Model")
	{
		return;
	}
	characterModel = (RBX::ModelInstance*)(character);
	bindMotors(characterModel);
}

void RBX::KeyboardController::bindMotors(RBX::Instance* character)
{
	
}

void RBX::KeyboardController::move()
{
	for (unsigned int i = 0; i < motors->size(); i++)
	{
		moveCharacter(motors->at(i));
	}
}

void RBX::KeyboardController::mv_update()
{

}

void RBX::KeyboardController::handleInput(G3D::UserInput* ui)
{
	Controller::handleInput(ui);
}
