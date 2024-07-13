
#include "part.h"
#include "model.h"

#include "stdout.h"

#include "workspace.h"
#include "datamodel.h"
#include "scriptcontext.h"

#include "camera.h"
#include "appmanager.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::Workspace>("Workspace")
        .constructor<>()
        .property_readonly("CurrentCamera", &RBX::Workspace::getCurrentCamera);
}

void RBX::Workspace::wakeUpModels()
{
    RBX::Instances* children;
    children = getChildren();

    for (unsigned int i = 0; i < children->size(); i++)
    {
        RBX::Instance* child;
        RBX::ModelInstance* model;
        child = children->at(i);
        model = dynamic_cast<RBX::ModelInstance*>(child);
        if (model)
        {
            model->buildJoints();
            model->createController();
        }
    }
}

void RBX::getPVInstances(RBX::Instances* instances, RBX::Instances* pvs)
{
    for (unsigned int i = 0; i < instances->size(); i++)
    {
        RBX::Instance* child = instances->at(i);

        if (RBX::IsA<RBX::PVInstance>(child))
            pvs->push_back(child);

        getPVInstances(child->getChildren(), pvs);
    }
}

void RBX::Workspace::onDescendentAdded(RBX::Instance* descendent)
{
    RBX::RunService::get()->onWorkspaceDescendentAdded(descendent);
    RBX::ScriptContext::get()->onWorkspaceDescendentAdded(descendent);
    RBX::View::get()->onWorkspaceDescendentAdded(descendent);
}

void RBX::Workspace::onDescendentRemoved(RBX::Instance* descendent)
{
    RBX::View::get()->onWorkspaceDescendentRemoved(descendent);
    RBX::ScriptContext::get()->onWorkspaceDescendentRemoved(descendent);
}

bool RBX::Workspace::setImageServerView()
{
    size_t numChildren;
    Instances* children;

    children = getChildren();
    numChildren = children->size();

    if (numChildren)
    {
        for (size_t i = 0; i < numChildren; i++)
        {
            RBX::ModelInstance* model = dynamic_cast<RBX::ModelInstance*>(children->at(i));
            if (model)
            {
                PVInstance* primaryPart = model->getPrimaryPart();
                if (primaryPart)
                {
                    CoordinateFrame cframe = primaryPart->getCFrame();
                    getCurrentCamera()->setImageServerViewNoLerp(cframe);
                }
            }
        }
    }

    return true;
}

RBX::Extents RBX::Workspace::computeCameraOwnerExtents()
{
    RBX::PartInstance* primaryPart;
    primaryPart = getPrimaryPart();

    if (primaryPart)
    {
        return primaryPart->getWorldExtents();
    }

    return RBX::Extents();
}

RBX::Workspace* RBX::Workspace::get()
{
    return RBX::Datamodel::getDatamodel()->workspace;
}

/* hacky method */

RBX::Camera* RBX::Workspace::getCurrentCamera()
{
    return RBX::AppManager::get()->getApplication()->getCamera();
}