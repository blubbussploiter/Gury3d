#include "keyboard.h"
#include "model.h"

void RBX::KeyboardController::moveCharacter(RBX::Instance* character)
{
}

void RBX::KeyboardController::bindCharacter(RBX::Instance* character)
{
	if (!RBX::IsA<ModelInstance>(character))
	{
		return;
	}

	RBX::ModelInstance* characterModel;
	characterModel = dynamic_cast<RBX::ModelInstance*>(character);

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
