#include "camera.h"

#include "application.h"
#include "appmanager.h"

GCamera::Frustum RBX::Camera::FrustumCulling::getFrustum()
{
	Camera* camera = RBX::Camera::get();

	Experimental::Application* app = RBX::AppManager::get()->getApplication();
	if (app)
	{
		RenderDevice* renderDevice = app->renderDevice;
		GCamera::Frustum frustum = camera->camera->frustum(renderDevice->getViewport());

		return frustum;
	}
}

float RBX::Camera::FrustumCulling::getNearZ()
{
	GCamera* camera = Camera::get()->camera;
	return camera->getNearPlaneZ();
}

RBX::Camera::FrustumCulling::State RBX::Camera::FrustumCulling::isBoxVisibleInCameraPlane(Box box)
{
	AABox aaBox;
	box.getBounds(aaBox);

	Vector3 v0, v1, v2, v3;
	for (int i = 0; i < 6; i++)
	{
		box.getFaceCorners(i, v0, v1, v2, v3);
		if (isPointVisibleInCameraPlane(v0)
			|| isPointVisibleInCameraPlane(v1)
			|| isPointVisibleInCameraPlane(v2)
			|| isPointVisibleInCameraPlane(v3))
		{
			return INSIDE;
		}
	}

	return OUTSIDE;
}

RBX::Camera::FrustumCulling::State RBX::Camera::FrustumCulling::isSphereVisibleInCameraPlane(Sphere sphere)
{
	GCamera::Frustum frustum = getFrustum();

	for (int f = 0; f < frustum.faceArray.size(); f++)
	{
		GCamera::Frustum::Face face = frustum.faceArray[f];
		Plane plane = face.plane;

		if (plane.distance(sphere.center) < sphere.radius || plane.distance(sphere.center) < -sphere.radius)
		{
			return OUTSIDE;
		}
	}

	return INSIDE;
}

bool RBX::Camera::FrustumCulling::isGeometryVisibleInCameraPlane(Render::Geometry* geometry)
{
	/*
	if (geometry->shape == Block)
	{
		if (isBoxVisibleInCameraPlane(geometry->getBox()) != OUTSIDE)
		{
			return true;
		}
	}
	else
	{
		if (isSphereVisibleInCameraPlane(geometry->getSphere()) != OUTSIDE)
		{
			return true;
		}
	}

	return false;
	*/
	return true;
}

bool RBX::Camera::FrustumCulling::isPointVisibleInCameraPlane(Vector3 point)
{
	/*
	GCamera::Frustum frustum = getFrustum();

	for (int f = 0; f < frustum.faceArray.size(); f++)
	{
		GCamera::Frustum::Face face = frustum.faceArray[f];
		Plane plane = face.plane;

		float distance = plane.distance(point);

		if (distance < -15.0f)
		{
			return false;
		}
	}
	*/
	return true;
}
