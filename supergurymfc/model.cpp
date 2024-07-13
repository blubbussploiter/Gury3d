#include "model.h"

#include "runservice.h"
#include "jointsservice.h"

#include "workspace.h"
#include "stdout.h"

#include "rightkeyboard.h"

RTTR_REGISTRATION
{
	rttr::registration::class_ <RBX::ModelInstance>("Model")
		 .constructor<>()
		 .property("primaryPart", &RBX::ModelInstance::getPrimaryPart, &RBX::ModelInstance::setPrimaryPart)(rttr::metadata("Type", RBX::Behavior))
		 .property("ControllerType", &RBX::ModelInstance::getController, &RBX::ModelInstance::setController)(rttr::metadata("Type", RBX::Behavior))
		 .method("breakJoints", &RBX::ModelInstance::breakJoints);
	rttr::registration::enumeration<RBX::ControllerTypes>("ControllerType")
		(
			rttr::value("None", RBX::None),
			rttr::value("KeyboardRight", RBX::KeyboardRight),
			rttr::value("KeyboardLeft", RBX::KeyboardLeft)
		);
}

void RBX::ModelInstance::createController()
{
	RBX::Instances* children;
	children = getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::Instance* child;
		child = children->at(i);
		if (child->getClassName() == "Model")
		{
			static_cast<RBX::ModelInstance*>(child)->makeController();
		}
	}
	makeController();
}

void RBX::ModelInstance::makeController()
{
	if (controller) return;
	switch (controllerType)
	{
	case KeyboardRight:
	{
		RightKeyboardController* rkc = new RightKeyboardController();
		rkc->bindCharacter(this);
		controller = rkc;
		break;
	}
	case KeyboardLeft:
	{
		KeyboardController* lkc = new KeyboardController();
		lkc->bindCharacter(this);
		controller = lkc;
		break;
	}
	default: return;
	}
	//RBX::ControllerService::get()->addController(controller);
}

void RBX::ModelInstance::drawControllerFlag(RenderDevice* rd, Color3 color)
{
	RBX::PVInstance* primaryPart = getPrimaryPart();
	if (!primaryPart) return;

	rd->setObjectToWorldMatrix(primaryPart->getCFrame());
	/* draw the flag stand */

	Draw::line(Line::fromTwoPoints(Vector3::zero(), Vector3(0, 4, 0)), rd, color);
}

void RBX::ModelInstance::render(RenderDevice* rd)
{
	Color3 color;
	switch (controllerType)
	{
	case KeyboardRight: color = Color3::BLUE; break;
	case KeyboardLeft: color = Color3::RED; break;
	case None: return;
	}
	drawControllerFlag(rd, color);
}

void RBX::ModelInstance::setPrimaryPartCFrame(CoordinateFrame cframe)
{
	if (getPrimaryPart())
	{
		primaryPart->setCFrame(cframe);
	}
}

CoordinateFrame RBX::ModelInstance::getPrimaryPartCFrame()
{
	if (getPrimaryPart())
	{
		return primaryPart->getCFrame();
	}
	return CoordinateFrame();
}

RBX::PartInstance* RBX::ModelInstance::getPrimaryPart()
{
	if (!primaryPart)
	{
		primaryPart = getPrimaryPartInternal();
	}
	return primaryPart;
}

RBX::Extents RBX::ModelInstance::computeCameraOwnerExtents()
{
	RBX::PartInstance* primaryPart;
	primaryPart = getPrimaryPart();

	if (primaryPart)
	{
		return primaryPart->getWorldExtents();
	}

	return RBX::Extents();
}

RBX::Extents RBX::ModelInstance::computeVisibleExtents()
{
	return getInstancesExtents(*getChildren());
}

void RBX::ModelInstance::translateInstances(Instances instances, PVInstance* rootPart, CoordinateFrame cframe)
{
	CoordinateFrame frame = rootPart->getCFrame();
	RBX::Instances* children = new Instances();

	RBX::getPVInstances(&instances, children);

	for (unsigned int i = 0; i < children->size(); i++)
	{
		PVInstance* pv = toInstance<PVInstance>(children->at(i));
		if (pv)
		{
			CoordinateFrame pvFrame = pv->getCFrame();
			Vector3 t = pvFrame.translation - frame.translation;
			Vector3 l = cframe.translation + t;
			Matrix3 r = pvFrame.rotation * cframe.rotation;
			pv->setCFrame(CoordinateFrame(r, l));
		}
	}
}

RBX::Extents RBX::ModelInstance::getInstancesExtents(Instances instances) /* modified version of: https://github.com/MaximumADHD/Super-Nostalgia-Zone/blob/4467be1ecec455a46cac919f26fddfbbc04169d9/join.client.lua#L59 */
{
	float minX = inf(), minY = inf(), minZ = inf();
	float maxX = -inf(), maxY = -inf(), maxZ = -inf();

	if (instances.size() == 2)
	{
		PVInstance* pvInstance;
		ModelInstance* modelInstance;

		modelInstance = toInstance<ModelInstance>(instances.at(0));
		pvInstance = toInstance<PVInstance>(instances.at(0));

		if (pvInstance)
		{
			return pvInstance->getWorldExtents();
		}

		if (modelInstance)
		{
			return modelInstance->computeVisibleExtents();
		}
	}

	for (unsigned int i = 0; i < instances.size(); i++)
	{
		RBX::Instance* instance = instances.at(i);
		RBX::ModelInstance* model;
		RBX::PVInstance* pvinstance;

		if ((model = dynamic_cast<RBX::ModelInstance*>(instance)) && model)
		{
			RBX::Extents modelExtents = model->computeVisibleExtents();
			Vector3 low = modelExtents.low;
			Vector3 size = modelExtents.size();

			minX = min(minX, low.x - size.x);
			minY = min(minY, low.y - size.y);
			minZ = min(minZ, low.z - size.z);

			maxX = max(minX, low.x + size.x);
			maxY = max(minY, low.y + size.y);
			maxZ = max(minZ, low.z + size.z);
		}
		if ((pvinstance = dynamic_cast<RBX::PVInstance*>(instance)) && pvinstance)
		{
			CoordinateFrame cf = pvinstance->getCFrame();
			Vector3 size = pvinstance->getSizeExternal();

			float
				sx = size.x,
				sy = size.y,
				sz = size.z;
			float
				x = cf.translation.x,
				y = cf.translation.y,
				z = cf.translation.z,
				r00 = cf.rotation.elt[0][0],
				r01 = cf.rotation.elt[0][1],
				r02 = cf.rotation.elt[0][2],
				r10 = cf.rotation.elt[1][0],
				r11 = cf.rotation.elt[1][1],
				r12 = cf.rotation.elt[1][2],
				r20 = cf.rotation.elt[2][0],
				r21 = cf.rotation.elt[2][1],
				r22 = cf.rotation.elt[2][2];

			float ws_x = (abs(r00) * sx + abs(r01) * sy + abs(r02) * sz) / 2;
			float ws_y = (abs(r10) * sx + abs(r11) * sy + abs(r12) * sz) / 2;
			float ws_z = (abs(r20) * sx + abs(r21) * sy + abs(r22) * sz) / 2;

			minX = min(minX, x - ws_x);
			minY = min(minY, y - ws_y);
			minZ = min(minZ, z - ws_y);

			maxX = max(maxX, x - ws_x);
			maxY = max(maxY, y - ws_y);
			maxZ = max(maxZ, z - ws_z);

		}
	}

	if (minX == inf())
	{
		return Extents();
	}

	return Extents(Vector3((minX + maxX) / 2,
		(minY + maxY) / 2,
		(minZ + maxZ) / 2), Vector3(maxX - minX,
			maxY - minY,
			maxZ - minZ));
}

RBX::PVInstance* RBX::ModelInstance::getRootPart(Instances i)
{
	RBX::PVInstance* result = 0;
	RBX::Instances* children = new Instances();

	float lastArea = -1;
	RBX::getPVInstances(&i, children);

	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::PVInstance* pv = (RBX::PVInstance*)(children->at(i));
		RBX::Extents extents = pv->getWorldExtents();
		float area = extents.area();
		if (area > lastArea)
		{
			lastArea = area;
			result = pv;
		}
	}
	children->clear();
	delete children;
	return result;
}

RBX::PartInstance* RBX::ModelInstance::getPrimaryPartInternal()
{
	return toInstance<PartInstance>(getRootPart(*getChildren()));
}

void RBX::ModelInstance::translate(CoordinateFrame cframe)
{
	translateInstances(*children, getPrimaryPart(), cframe);
}

/* joints */

void RBX::ModelInstance::buildJoints()
{

}

void RBX::ModelInstance::breakJoints()
{
	Instances* instances = new Instances();
	getPVInstances(getChildren(), instances);

	for (unsigned int i = 0; i < instances->size(); i++)
	{
		PVInstance* pv = toInstance<PVInstance>(instances->at(i));
		Body* body = pv->primitive->body;

		if (body)
		{
			void* ud = body->getUserdata();
			Connector* connector = (Connector*)(ud);
			if(connector) connector->unlink();
		}
	}
}