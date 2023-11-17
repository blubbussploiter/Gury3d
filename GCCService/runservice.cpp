#include "stdout.h"

#include "scene.h"
#include "workspace.h"
#include "jointservice.h"

#include "datamodel.h"
#include "players.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::RunService>("RunService")
        .constructor<>()
        .method("pause", &RBX::RunService::stop)
        .method("run", &RBX::RunService::run);
}


void RBX::RunService::run()
{
    RBX::Network::Player* localPlayer;
    localPlayer = RBX::Network::getPlayers()->localPlayer;

    if (localPlayer)
    {
        localPlayer->setAsController();
    }

    /* add here, check for jointservice build joints on load, 
    isRunning = true; */
    reset();
}

void RBX::RunService::stop()
{
    isRunning = false;
}

void RBX::RunService::reset()
{
    RBX::Scene::singleton()->updatePhysicsObjects();
    /* add here, check for jointservice build joints on load,
    RBX::Workspace::singleton()->buildJoints(); */
    RBX::StandardOut::print(RBX::MESSAGE_INFO, "RunService::run()");
    isRunning = true;
}

void RBX::RunService::update()
{
    for (int i = 0; i < 6; i++)
    {
        physics->update();
    }

    RBX::Scene::singleton()->updatePhysicsObjects();
    RBX::Scene::singleton()->updateSteppables();

}

void RBX::RunService::heartbeat()
{
    update();
}

void RBX::RunService::updateSteppers()
{
    for (unsigned int i = 0; i < steppers->size(); i++)
    {
        steppers->at(i)->onStep();
    }
}

void RBX::RunService::workspaceOnDescendentAdded(RBX::Instance* descendent)
{
    if (descendent->isSteppable)
    {
        steppers->push_back(descendent);
    }
}

RBX::RunService* RBX::RunService::singleton()
{
    return RBX::Datamodel::getDatamodel()->runService;
}