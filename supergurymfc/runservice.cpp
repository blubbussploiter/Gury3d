#include "stdout.h"

#include "scene.h"
#include "workspace.h"

#include "scriptcontext.h"
#include "jointsservice.h"

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

    reset();

    if (localPlayer)
    {
        localPlayer->setAsController();
    }

    if (scriptContext)
    {
        //scriptContext->runScripts();
    }

    isRunning = true;
}

void RBX::RunService::stop()
{
    isRunning = false;
}

void RBX::RunService::reset()
{
    RBX::Scene::singleton()->initializeKernel();
    RBX::JointsService::singleton()->buildGlobalJoints();
    RBX::JointsService::singleton()->buildConnectors();
}

void RBX::RunService::update()
{
    RBX::Scene::singleton()->updateSteppables();

    for (int i = 0; i < 8; i++)
    {
         Kernel::get()->step(0.025f);
    }
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

void RBX::RunService::onWorkspaceDescendentAdded(RBX::Instance* descendent)
{

}

RBX::RunService* RBX::RunService::singleton()
{
    return RBX::Datamodel::getDatamodel()->runService;
}