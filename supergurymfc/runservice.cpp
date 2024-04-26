
#include <queue>

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
    if (isRunning) return;

    RBX::Network::Player* localPlayer;
    localPlayer = RBX::Network::getPlayers()->localPlayer;

    Scene::singleton()->saveStartPVs();
    Kernel::get()->spawnWorld();

    if (!hasStarted)
    {
        reset();
        hasStarted = 1;
    }
    else
    {
        restartPvs();
    }

    if (localPlayer)
    {
        localPlayer->setAsController();
    }

    if (scriptContext)
    {
        //scriptContext->runScripts();
    }

    isRunning = true;
    isPaused = false;
}

void RBX::RunService::pause()
{
    stop();
    isPaused = true;
}

void RBX::RunService::stop()
{
    isRunning = false;
    isPaused = false;
}

void RBX::RunService::reset()
{
    if (!hasStarted)
    {
       JointsService::singleton()->buildGlobalJoints();
       JointsService::singleton()->buildConnectors();
    }
    else
    {
        stop();
        shouldReset = 1;
    }
}

void RBX::RunService::update()
{

    if (isRunning)
    {

        for (int i = 0; i < 8; i++)
        {
            Kernel::get()->step();
        }

        RBX::Scene::singleton()->updateSteppables();
    }

    /* reset pvs */

    if (shouldReset)
    {
        resetPvs();
        shouldReset = 0;
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

void RBX::RunService::resetPvs()
{
    IRenderableArray scene = Scene::singleton()->getArrayOfObjects();
    for (unsigned int i = 0; i < scene.size(); i++)
    {
        PVInstance* pv = toInstance<PVInstance>(scene.at(i));
        if (pv)
        {
            pv->resetPV();
        }
    }
}

void RBX::RunService::restartPvs()
{
    IRenderableArray scene = Scene::singleton()->getArrayOfObjects();
    for (unsigned int i = 0; i < scene.size(); i++)
    {
        PVInstance* pv = toInstance<PVInstance>(scene.at(i));
        if (pv)
        {
            pv->restartPV();
        }
    }
}

void RBX::RunService::onWorkspaceDescendentAdded(RBX::Instance* descendent)
{

}

RBX::RunService* RBX::RunService::singleton()
{
    return RBX::Datamodel::getDatamodel()->runService;
}