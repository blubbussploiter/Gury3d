
#include <thread>

#include "instance.h"
#include "workspace.h"

#include "camera.h"
#include "sounds.h"

#include "appmanager.h"

static RBX::Sound* switch3 = RBX::Sound::fromFile(GetFileInPath("/content/sounds/SWITCH3.wav"));

RTTR_REGISTRATION
{
    rttr::registration::class_ <RBX::Camera>("Camera")
         .constructor<>()
        .property("Focus", &RBX::Camera::getFocus, &RBX::Camera::setFocus)
        .property("CoordinateFrame", &RBX::Camera::getCoordinateFrame, &RBX::Camera::setCoordinateFrame);
}

void RBX::Camera::tiltUp(double deg, bool enactedByZoom)
{
    pan(&cframe, 0, toRadians(deg), 1, 0.41f);
    if(!enactedByZoom) switch3->play();
}

void RBX::Camera::tiltDown(double deg, bool enactedByZoom)
{
    pan(&cframe, 0, toRadians(-deg), 1, 0.41f);
    if (!enactedByZoom) switch3->play();
}

void RBX::Camera::cam_zoom(bool inout)
{
    if(cameraType == Follow) isInFirstPerson = (zoom <= 1.5f);

    if (inout)
    {
        Zoom(1);
        return;
    }

    Zoom(-1);
}

void RBX::Camera::update(UserInput* input)
{

    if (focusPart &&
        focusPosition != focusPart->getPosition())
    {
        focusPosition = focusPart->getPosition();
    }

    pan(&cframe, 0, 0);
    camera->setCoordinateFrame(cframe);
}

void RBX::Camera::move()
{
    if (!moving())
        return;

    switch (dir())
    {
        case RBX::Forward:
        {
            cframe.translation += cframe.lookVector() * getSpeed();
            break;
        }
        case RBX::Backwards:
        {
            cframe.translation -= cframe.lookVector() * getSpeed();
            break;
        }
        case RBX::Right:
        {
            cframe.translation += cframe.rightVector() * getSpeed();
            break;
        }
        case RBX::Left:
        {
            cframe.translation -= cframe.rightVector() * getSpeed();
            break;
        }
    }

    setFrame(cframe);
}

void RBX::Camera::zoomExtents()
{
    zoomExtents(RBX::Workspace::singleton()->computeVisibleExtents());
}

void RBX::Camera::zoomExtents(RBX::Extents extents, bool lerp)
{
    Vector3 center, high, low;
    Vector3 cameraPosition;

    center = extents.getCenter();

    high = extents.high;
    low = extents.low;

    cameraPosition = center + (high);

    CoordinateFrame cameraFrame;

    cameraFrame = cameraPosition;
    cameraFrame.lookAt(center);

    camera->setCoordinateFrame(cameraFrame);
}

void RBX::Camera::setImageServerViewNoLerp(CoordinateFrame modelCoord)
{
    zoomExtents();
}

RBX::ICameraOwner* RBX::Camera::getCameraOwner()
{
    RBX::Instance* parent; // esi
    RBX::ICameraOwner* result; // eax

    parent = getParent();
    if (!parent)
        return 0;
    while (1)
    {
        result = dynamic_cast<RBX::ICameraOwner*>(parent);
        if (result)
            break;
        parent = parent->getParent();
        if (!parent)
            return 0;
    }
    return result;
}


/* same as `workspace.CurrentCamera` */

RBX::Camera* RBX::Camera::singleton()
{
    return RBX::AppManager::singleton()->getApplication()->getCamera();
}
