#ifndef CAMERA_H
#define CAMERA_H

/* I am shit at math and whatever this is, so I took most of the code from https://github.com/Vulpovile/Blocks3D/blob/0b8847cd8e7d5726870e9f65c558872e0eaf477e/src/source/CameraController.cpp */

#include <G3DAll.h>

#include "ICameraOwner.h"

#include "pvinstance.h"
#include "controller.h"
#include "part.h"

#define CAM_ZOOM_MIN 0.5f
#define CAM_ZOOM_MAX 1000.f /* should be dependant on extents */

namespace RBX
{
	enum CameraType
	{
		Fixed,
		Follow
	};

	class Camera : 
		public RBX::Controller
	{
		RTTR_ENABLE(RBX::Controller)
	private:
		CoordinateFrame startFrame;
		Vector3 startFocus;
	public:

		GCamera* camera;
		CameraType cameraType;

		float yaw, pitch, zoom;

		RBX::PartInstance* focusPart;

		Vector3 focusPosition;
		Vector3 translation;

		CoordinateFrame cframe;

		/* blocks3d stuff */

		void lookAt(const Vector3& position);
		void setFrame(const CoordinateFrame& cf);

		void occlude();

		CoordinateFrame getCoordinateFrame();
		CoordinateFrame getFocus() { return focusPosition; }

		void characterFade();

		void setCoordinateFrame(CoordinateFrame cf)
		{
			if (startFrame.isIdentity())
			{
				startFrame = cf;
			}
			cframe = cf;
		}

		void setFocus(CoordinateFrame cf)
		{
			if (!startFocus)
			{
				startFocus = cf.translation;
			}
			focusPosition = cf.translation;
		}

		void refreshZoom(const CoordinateFrame& frame);

		void pan(CoordinateFrame* frame, float spdX, float spdY);
		void panLock(CoordinateFrame* frame, float spdX, float spdY);

		void Zoom(short delta);

		/* My stuff */

		void tiltUp(double deg = 25, bool enactedByZoom = 0);
		void tiltDown(double deg = 25, bool enactedByZoom = 0);

		void cam_zoom(bool inout);

		void reset();

		void update(bool rightMouseDown);
		void follow();

		void setCamera(GCamera* c) { camera = c; }
		GCamera* getCamera() { return camera; }

		float getLerp() { return 0.49999998f; }

		virtual void move();

		void zoomExtents();
		void zoomExtents(RBX::Extents extents, bool lerp=1);

		void setImageServerViewNoLerp(CoordinateFrame modelCoord);
		RBX::ICameraOwner* getCameraOwner();

		static RBX::Camera* singleton();

		Camera() : focusPosition(Vector3(0, 0, 0)), yaw(0.f), pitch(0.f), zoom(14.f)
		{
			cameraType = CameraType::Fixed;
			focusPart = 0;
			isParentLocked = 1;
			setSpeed(2.4f);
			setClassName("Camera");
			setName("Camera");
		}

		virtual ~Camera() {}
	};
}

#endif