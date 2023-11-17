#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <G3DAll.h>
#include <vector>

#include "ICameraOwner.h"

#include "model.h"
#include "camera.h"

#include "extents.h"
#include "view.h"

#include "service.h"

namespace RBX
{

	class Workspace :
		public RBX::ModelInstance,
		public RBX::Service<Workspace>
	{
	private:
		Camera* currentCamera;
	public:
		/* deprecated, use Datamodel->workspace */

		static Workspace* singleton();

		void wakeUpModels();

		void onDescendentAdded(RBX::Instance* descendent);
		void onDescendentRemoved(RBX::Instance* descendent);

		Camera* getCurrentCamera();
		void setCurrentCamera(Camera* camera);

		bool setImageServerView();
		Box getBoundingBox()
		{
			return Box(Vector3::zero(), Vector3::zero());
		}

		virtual RBX::ModelInstance* getModel() { return this; }
		virtual RBX::Extents computeCameraOwnerExtents();

		Workspace()
		{
			currentCamera = nullptr;

			setClassName("Workspace");
			setName("Workspace");

			isParentLocked = 1;

		}

		RTTR_ENABLE(ModelInstance)
	};
	void getPVInstances(RBX::Instances* instances, RBX::Instances* pvs);
}

#endif
