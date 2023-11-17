
#include "part.h"
#include "model.h"

#include "stdout.h"

#include "workspace.h"
#include "datamodel.h"

#include "camera.h"
#include "appmanager.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::Workspace>("Workspace")
        .constructor<>()
        .property("CurrentCamera", &RBX::Workspace::getCurrentCamera, &RBX::Workspace::setCurrentCamera);
}

void RBX::Workspace::wakeUpModels()
{
    RBX::Instances* children;
    children = getChildren();

    for (unsigned int i = 0; i < children->size(); i++)
    {
        RBX::Instance* child;
        child = children->at(i);
        if (child->getClassName() == "Model")
        {
            static_cast<RBX::ModelInstance*>(child)->buildJoints();
            static_cast<RBX::ModelInstance*>(child)->createController();
        }
    }
}

void updatePVInstances(RBX::Instances* PVInstances)
{
    RBX::PVInstance* part;
    for (size_t i = 0; i < PVInstances->size(); i++)
    {
        RBX::Instance* child = PVInstances->at(i);
        if (child->isRenderable || child->isAffectedByPhysics)
        {
            if (RBX::IsA<RBX::PVInstance>(child))
            {
                part = (RBX::PVInstance*)child;
                RBX::RunService::singleton()->getPhysics()->createBody(part);
            }
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
    RBX::View::singleton()->onWorkspaceDescendentAdded(descendent);
}

void RBX::Workspace::onDescendentRemoved(RBX::Instance* descendent)
{
    RBX::View::singleton()->onWorkspaceDescendentRemoved(descendent);
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

RBX::Workspace* RBX::Workspace::singleton()
{
    return RBX::Datamodel::getDatamodel()->workspace;
}

/* hacky method */

RBX::Camera* RBX::Workspace::getCurrentCamera()
{
    return RBX::AppManager::singleton()->getApplication()->getCamera();
}

void RBX::Workspace::setCurrentCamera(Camera* camera)
{

}