#ifndef MODEL_H
#define MODEL_H

#include "ICameraOwner.h"
#include "ISelectable.h"

#include "controller.h"
#include "part.h"

#include "stdout.h"

namespace RBX
{
	class ModelInstance :
		public RBX::Instance,
		public RBX::ISelectable,
		public ICameraOwner
	{
	private:
		RBX::ControllerTypes controllerType;
		RBX::Controller* controller;
	public:

		RBX::PartInstance* primaryPart;

		void breakJoints();
		void buildJoints();

		void createController(); /* recursive */
		void makeController(); /* non recursive */

		void setController(ControllerTypes c) 
		{
			controllerType = c;
		}

		void setPrimaryPartCFrame(CoordinateFrame cframe);
		CoordinateFrame getPrimaryPartCFrame();

		ControllerTypes getController() { return controllerType; }
		RBX::PartInstance* getPrimaryPartInternal();

		RBX::PartInstance* getPrimaryPart();

		virtual RBX::ModelInstance* getModel() { return this; }
		virtual RBX::Extents computeCameraOwnerExtents();
		RBX::Extents computeVisibleExtents();

		Box getBoundingBox()
		{
			Vector3 low, high;
			for (RBX::Instance* i : *getChildren())
			{
				RBX::ISelectable* s = dynamic_cast<RBX::ISelectable*>(i);
				if (s)
				{
					Box b = s->getBoundingBox();
					AABox ab;

					b.getBounds(ab);

				}
			}
			return Box(low, high);
		}

		ModelInstance()
		{
			controllerType = ControllerTypes::None;
			controller = nullptr;
			primaryPart = nullptr;
			setClassName("Model");
			setName("Model");
		}

		virtual ~ModelInstance() { breakJoints(); }
		RTTR_ENABLE(Instance)
	};

}

#endif