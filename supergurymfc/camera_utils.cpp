
#include "camera.h"
#include "sounds.h"

#include "ray.h"

#include "stdout.h"
#include "players.h"

static RBX::Sound* switch3 = RBX::Sound::fromFile(GetFileInPath("/content/sounds/SWITCH3.wav"));

void RBX::Camera::lookAt(const Vector3& position)
{
	const Vector3 look = (position - cframe.translation);
	yaw = aTan2(look.x, -look.z);

	pitch = -aTan2(look.y, distance(look.x, look.z));
	CoordinateFrame frame = CoordinateFrame();
	frame.translation = cframe.translation;
	frame.rotation = Matrix3::fromEulerAnglesZYX(0, -yaw, -pitch);
	cframe = frame;
}

void RBX::Camera::setFrame(const CoordinateFrame& cf)
{
	Vector3 look = cf.getLookVector();
	cframe = cf;
	lookAt(cframe.translation + look);
	focusPosition = cframe.translation + cframe.lookVector() * zoom;
}

void RBX::Camera::occlude()
{
	if (focusPart)
	{
		RBX::World::Ray* ray;
		RBX::Network::Player* player;
		RBX::ISelectable* selectable;
		RBX::PartInstance* part;
		RBX::Instance* parent;

		Vector3 startPos, negLook, hit;

		player = RBX::Network::getPlayers()->localPlayer;
		parent = focusPart->getParent();

		if ((player && player->character)
			&& parent && parent != player->character) return;

		startPos = cframe.translation;
		negLook = focusPart->getPosition();

		ray = new World::Ray(startPos, negLook);
		selectable = World::getPartFromG3DRay<Instance>(ray->g3dRay, hit);

		if ((part = dynamic_cast<RBX::PartInstance*>(selectable)) && part)
		{
			if (part->getParent() != parent)
			{
				Vector3 pos = ray->p;
				float distance = (pos - startPos).magnitude();

				cframe.translation += (cframe.lookVector() * distance);
			}
		}
	}
}

CoordinateFrame RBX::Camera::getCoordinateFrame()
{
	CoordinateFrame cf;
	cf.translation = translation;
	cf.rotation = Matrix3::fromEulerAnglesZYX(0, -yaw, -pitch);
	return cf;
}

void RBX::Camera::refreshZoom(const CoordinateFrame& frame)
{
	CoordinateFrame zoomFrame = focusPosition - frame.lookVector() * zoom;
	zoomFrame.lookAt(focusPosition);
	setFrame(zoomFrame);
}

void RBX::Camera::pan(CoordinateFrame* frame, float spdX, float spdY, bool lookAt)
{
	Vector3 pos;

	yaw += spdX;
	pitch += spdY;

	if (pitch > 1.4f) pitch = 1.4f;
	if (pitch < -1.4f) pitch = -1.4f;

	pos = Vector3(sin(-yaw) * zoom * cos(pitch), sin(pitch) * zoom, cos(-yaw) * zoom * cos(pitch)) + focusPosition;

	frame->translation = pos;
	if (lookAt)
	{
		frame->lookAt(focusPosition);
	}
}

void RBX::Camera::panLock(CoordinateFrame* frame, float spdX, float spdY)
{

	int sign = 0;

	yaw = toDegrees(yaw);
	if ((((yaw - fmod(yaw, 45)) / 45) * 45) < 0)
	{
		sign = 1;
	}
	yaw = fabs(yaw);
	yaw = ((yaw - fmod(yaw, 45)) / 45) * 45;
	yaw = toRadians(yaw);

	if (sign == 1)
	{
		yaw = yaw * -1;
	}

	yaw += spdX;
	pitch += spdY;

	if (pitch > 1.4f) pitch = 1.4f;
	if (pitch < -1.4f) pitch = -1.4f;
	frame->translation = Vector3(sin(-yaw) * zoom * cos(pitch), sin(pitch) * zoom, cos(-yaw) * zoom * cos(pitch)) + focusPosition;
	frame->lookAt(focusPosition);
}

void RBX::Camera::Zoom(short delta)
{
	if (cameraType != Follow)
	{
		switch3->play();
	}
	if (oldZoom > zoom)
	{
		zoom = oldZoom;
		oldZoom = 0;
		pan(&cframe, 0, 0);
	}

	if (delta>0) { // Mouse wheel up
		CoordinateFrame zoomFrame = cframe + cframe.lookVector() * (zoom * 0.2f);
		zoom = (zoomFrame.translation - focusPosition).magnitude();
		if (zoom > CAM_ZOOM_MIN)
		{
			setFrame(zoomFrame);
		}
		else
		{
			zoom = CAM_ZOOM_MIN;
			refreshZoom(cframe);
		}
		if (cameraType == Follow)
		{
			tiltDown(5);
		}
	}
	else 
	{
		if (canZoom(0))
		{
			CoordinateFrame zoomFrame = cframe - cframe.lookVector() * (zoom * 0.2f);
			zoom = (zoomFrame.translation - focusPosition).magnitude();
			if (zoom < CAM_ZOOM_MAX)
			{
				setFrame(zoomFrame);
			}
			else
			{
				zoom = CAM_ZOOM_MAX;
				refreshZoom(cframe);
			}
		}
		if (cameraType == Follow)
		{
			tiltUp(5);
		}
	}
}
