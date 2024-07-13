
#include <thread>

#include "instance.h"
#include "workspace.h"

#include "camera.h"
#include "sounds.h"

#include "appmanager.h"
#include "players.h"

#include "humanoid.h"
#include "selection.h"

POINT oldMouse;
POINT mouse;

RBX::Sound* RBX::Camera::switch3 = RBX::Sound::fromFile(GetFileInPath("/content/sounds/SWITCH3.wav"));

RTTR_REGISTRATION
{
    rttr::registration::class_ <RBX::Camera>("Camera")
         .constructor<>()
        .property("Focus", &RBX::Camera::getFocus, &RBX::Camera::setFocus)
        .property("CoordinateFrame", &RBX::Camera::getCoordinateFrame, &RBX::Camera::setCoordinateFrame);
}

void RBX::Camera::tiltRight(double deg)
{
    pan(&cframe, toRadians(deg), 0);
    switch3->play();
}

void RBX::Camera::tiltLeft(double deg)
{
    pan(&cframe, toRadians(-deg), 0);
    switch3->play();
}

void RBX::Camera::tiltUp(double deg)
{
    pan(&cframe, 0, toRadians(deg));
    switch3->play();
}

void RBX::Camera::tiltDown(double deg)
{
    pan(&cframe, 0, toRadians(-deg));
    switch3->play();
}

void RBX::Camera::characterFade()
{
    RBX::Network::Player* player;
    player = RBX::Network::getPlayers()->localPlayer;

    if (!focusPart) return;
    if (!player || player && !player->character) return;

    RBX::ModelInstance* character = player->character;
    RBX::Humanoid* humanoid = Humanoid::modelIsCharacter(character);

    if (!humanoid) return;

    Vector3 pos = cframe.translation;
    Vector3 ppos = focusPart->getPosition();

    float dist = (ppos - pos).magnitude();

    /* semi deep */

    if (dist <= 5)
    {
        humanoid->setLocalTransparency(0.5f);

        /* deeper */

        if (dist <= 2.5)
        {
            humanoid->setLocalTransparency(1.0f);
        }
    }
    else
    {
        humanoid->setLocalTransparency(0.0f);
    }
}


void RBX::Camera::cam_zoom(bool inout)
{
    if (inout)
    {
        Zoom(1);
        return;
    }

    Zoom(-1);
}

void RBX::Camera::reset()
{
    setCoordinateFrame(startFrame);
    setFocus(startFocus);
}

void RBX::Camera::follow()
{
    if (!focusPart) return;
    CoordinateFrame focus = focusPart->getCoordinateFrame();
    focusPosition = focus.translation;
    pan(&cframe, 0, 0, 1);
}

void RBX::Camera::update(bool rightMouseDown)
{
    Vector3 pos;
    float smoothness;

    panning = rightMouseDown;
    
    switch (cameraType)
    {
        case Follow:
        {
            smoothness = 0.95f;
            characterFade();
            follow();
            break;
        }
        default: 
        {
            smoothness = 0.55f;
            break;
        }
    }

    if (rightMouseDown)
    {
        if (oldMouse.x != 0 && oldMouse.y != 0)
        {
            GetCursorPos(&mouse);
            pan(&cframe, (mouse.x - oldMouse.x) / 80.f, (mouse.y - oldMouse.y) / 80.f);
            SetCursorPos(oldMouse.x, oldMouse.y);
        }
    }

    GetCursorPos(&oldMouse);

    CoordinateFrame current = camera->getCoordinateFrame();
    if (cframe != current)
    {
        camera->setCoordinateFrame(current.lerp(cframe, smoothness));
    }

}

void RBX::Camera::move()
{
    if (!moving())
        return;

    for (unsigned int i = 0; i < directions.size(); i++)
    {
        MovementDirections dir = directions.at(i);
        switch (dir)
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
    }

    camera->setCoordinateFrame(cframe);
    setFrame(cframe);
}

void RBX::Camera::lookAtSelected()
{
    std::vector<RBX::ISelectable*> selected = RBX::Selection::get()->selection;
    Vector3 position;

    position = ISelectable::getCenterOfSelectableObjects(selected);

    lookAt(position);
}

void RBX::Camera::zoomExtents()
{
    zoomExtents(RBX::Workspace::singleton()->computeVisibleExtents());
}

void RBX::Camera::zoomExtents(RBX::Extents extents, bool lerp)
{
    Vector3 size;

    float magnitude;
    size = extents.size();

    magnitude = size.magnitude() * 0.5f;

    zoom = magnitude;
    pan(&cframe, 0, 0); /* update for zoom */
}

void RBX::Camera::setImageServerViewNoLerp(CoordinateFrame modelCoord)
{

    Vector3 look;
    Vector3 Column;

    CoordinateFrame lookCoord;
    ICameraOwner* owner;

    float v5, v6, v7, v8, v9, v10;

    Column = modelCoord.lookVector();
    v5 = -1.0f * Column.x;
    v6 = Column.z * -1.0f;

    if (fabs(Column.y * -1.0f) <= 0.95f)
    {
        v10 = 1.0f / sqrt(v5 * v5 + v6 * v6);
        v9 = v6 * v10;
        v8 = v5 * v10;
        v7 = 0.0f * v10;
    }
    else
    {
        v7 = 0;
        v8 = 0;
        v9 = -1;
    }

    look = Vector3(-1.0f, v8, v7);
    lookCoord.lookAt(look);

    lookCoord.rotation *= Matrix3::fromEulerAnglesZXY(0.698131701f, 0.523598776f, 0.0f);

    look = lookCoord.lookVector();
    Vector3 mod = modelCoord.translation + (look * 10);

    lookCoord.translation = Vector3(mod.x, mod.y, mod.z);
    lookCoord.lookAt(modelCoord.translation);

    cframe = lookCoord;

    if ((owner = getCameraOwner()) && owner)
    {
        zoomExtents(owner->computeCameraOwnerExtents(), 0);
    }

    focusPosition = modelCoord.translation;

    pan(&cframe, 0, 0);
    update(0);
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

void RBX::Camera::doCameraCollisionLogic()
{
    if (!focusPart) return;

    CoordinateFrame cameraFrame = camera->getCoordinateFrame();
    Ray ray = Ray::fromOriginAndDirection(cameraFrame.translation, -cameraFrame.lookVector());

    Vector3 hit = World::getHitFromRay<Instance>(ray, true, *focusPart->getParent()->getChildren());
    if (hit != Vector3::zero())
    {
        float distance = (cameraFrame.translation - hit).magnitude();

        if (distance < CAM_ZOOM_MAX && distance > CAM_ZOOM_MIN)
        {
            if (distance <= 15)
            {
                if (zoom >= distance)
                {
                    if (distance >= oldZoom)
                    {
                        oldZoom = zoom;
                    }
                    zoom = distance;
                }
            }
            else
            {
                if (oldZoom > zoom && oldZoom < distance) /* main problem child thing */
                {
                    zoom = oldZoom;
                }
            }

        }
    }
}

bool RBX::Camera::canZoom(bool inout)
{
    
    if (cameraType != Follow || !focusPart) return 1;

    CoordinateFrame cameraFrame = camera->getCoordinateFrame();
    Ray ray;

    if (inout)
    {
        ray = Ray::fromOriginAndDirection(cframe.translation, cframe.lookVector());
    }
    else
    {
        ray = Ray::fromOriginAndDirection(cframe.translation, -cframe.lookVector());
    }

    Vector3 hit = World::getHitFromRay<Instance>(ray, true, *focusPart->getParent()->getChildren());
    if (hit != Vector3::zero())
    {
        return ((cameraFrame.translation - hit).magnitude() > 5);
    }
    else
    {
        return 1;
    }

    return 0;
}

/* same as `workspace.CurrentCamera` */

RBX::Camera* RBX::Camera::singleton()
{
    return RBX::AppManager::singleton()->getApplication()->getCamera();
}
