
#include <thread>

#include "instance.h"
#include "workspace.h"

#include "camera.h"
#include "sounds.h"

#include "appmanager.h"
#include "players.h"

#include "humanoid.h"

POINT oldMouse;
POINT mouse;

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

void RBX::Camera::characterFade()
{
    RBX::Network::Player* player;
    player = RBX::Network::getPlayers()->localPlayer;

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

void RBX::Camera::update(UserInput* input) /* jesus fucking christ CLEAN UP THIS CODE */
{

    Vector3 pos;
    float lerp = 0.30f;

    isUsingRightMouse = input->keyDown(SDL_RIGHT_MOUSE_KEY);

    if (cameraType == Follow)
    {
        if (focusPart &&
            focusPosition != focusPart->getPosition())
        {
            focusPosition = focusPart->getPosition() + Vector3(0, 0.25f, 0);
        }
        focusPosition += Vector3(0, 0.25f, 0);
        lerp = 0.95f;
    }

    if (isUsingRightMouse)
    {
        if (oldMouse.x != 0 && oldMouse.y != 0)
        {
            GetCursorPos(&mouse);
            pan(&cframe, (mouse.x - oldMouse.x) / 80.f, (mouse.y - oldMouse.y) / 80.f, 1, lerp);
            SetCursorPos(oldMouse.x, oldMouse.y);
        }
        if (cameraType == Follow)
        {
            characterFade();
        }
    }

    GetCursorPos(&oldMouse);

    pan(&cframe, 0, 0, 1, lerp);
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
        case RBX::BackwardsRight:
        case RBX::ForwardRight:
        case RBX::Right:
        {
            cframe.translation += cframe.rightVector() * getSpeed();
            break;
        }
        case RBX::BackwardsLeft:
        case RBX::ForwardLeft:
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
    Vector3 size, center;
    CoordinateFrame cameraPosition;

    center = extents.low;
    size = extents.size();

    float mag = size.magnitude() / 4;

    CoordinateFrame cameraFrame = camera->getCoordinateFrame();

    cameraPosition = (center - (cameraFrame.lookVector() * mag));
    cameraPosition.lookAt(center);

    cframe = cameraPosition;
    focusPosition = center;

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


/* same as `workspace.CurrentCamera` */

RBX::Camera* RBX::Camera::singleton()
{
    return RBX::AppManager::singleton()->getApplication()->getCamera();
}
