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
		public RBX::Render::Renderable,
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

		void drawControllerFlag(RenderDevice* rd, Color3 color);
		void render(RenderDevice* rd);

		void setPrimaryPartCFrame(CoordinateFrame cframe);
		CoordinateFrame getPrimaryPartCFrame();

		ControllerTypes getController() { return controllerType; }
		RBX::PartInstance* getPrimaryPartInternal();

		RBX::PartInstance* getPrimaryPart();
		void setPrimaryPart(RBX::PartInstance* part) { primaryPart = part; }

		virtual RBX::ModelInstance* getModel() { return this; }
		virtual RBX::Extents computeCameraOwnerExtents();

		RBX::Extents computeVisibleExtents();

		void translate(CoordinateFrame cframe);
		
		static void translateInstances(Instances i, PVInstance* rootPart, CoordinateFrame cframe);
		static Extents getInstancesExtents(Instances i);
		static PVInstance* getRootPart(Instances i);

		SelectableBox getBoundingBox()
		{
			return SelectableBox();
		}

		ModelInstance()
		{
			controllerType = ControllerTypes::None;
			controller = nullptr;
			primaryPart = nullptr;
			unaffectedByLight = true;
			setClassName("Model");
			setName("Model");
		}

		virtual ~ModelInstance() { breakJoints(); }
		RTTR_ENABLE(Instance)
	};

}

#endif