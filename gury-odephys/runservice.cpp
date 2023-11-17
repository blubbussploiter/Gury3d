#include "stdout.h"

#include "scene.h"
#include "workspace.h"
#include "jointservice.h"

#include "datamodel.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::RunService>("RunService")
        .constructor<>()
        .method("pause", &RBX::RunService::stop)
        .method("run", &RBX::RunService::run);
}


void RBX::RunService::run()
{
    isRunning = true;
    reset();
}

void RBX::RunService::stop()
{
    isRunning = false;
}

void RBX::RunService::reset()
{
    RBX::Scene::singleton()->updatePhysicsObjects();
    RBX::Workspace::singleton()->buildJoints();
}

void RBX::RunService::update()
{
    physics->update();

    RBX::JointService::singleton()->update();

    RBX::Scene::singleton()->updatePhysicsObjects();
    RBX::Scene::singleton()->updateSteppables();

}

void RBX::RunService::heartbeat()
{
    update();
    //updateSteppers();
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