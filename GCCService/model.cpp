#include "model.h"

#include "runservice.h"
#include "jointservice.h"

#include "workspace.h"
#include "stdout.h"

#include "rightkeyboard.h"

RTTR_REGISTRATION
{
	rttr::registration::class_ <RBX::ModelInstance>("Model")
		 .constructor<>()
		 .property("ControllerType", &RBX::ModelInstance::getController, &RBX::ModelInstance::setController)(rttr::metadata("Type", RBX::Behavior));
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
	//RBX::ControllerService::singleton()->addController(controller);
}

void RBX::ModelInstance::setPrimaryPartCFrame(CoordinateFrame cframe)
{
	getPrimaryPart()->setCFrame(cframe);
}

CoordinateFrame RBX::ModelInstance::getPrimaryPartCFrame()
{
	return getPrimaryPart()->getCFrame();
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
	float minX = inf(), minY = inf(), minZ = inf();
	float maxX = -inf(), maxY = -inf(), maxZ = -inf();

	for (unsigned int i = 0; i < getChildren()->size(); i++)
	{
		RBX::Instance* instance = getChildren()->at(i);
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

	if(minX == inf()) 
	{
		return Extents();
	}

	return Extents(Vector3((minX + maxX) / 2,
		(minY + maxY) / 2,
		(minZ + maxZ) / 2), Vector3(maxX - minX,
			maxY - minY,
			maxZ - minZ));
}

RBX::PartInstance* RBX::ModelInstance::getPrimaryPartInternal()
{
	RBX::PartInstance* result = 0;
	RBX::Instances* children = new Instances();

	float lastArea = -1;
	RBX::getPVInstances(getChildren(), children);

	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::PartInstance* pv = (RBX::PartInstance*)(children->at(i));
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

/* joints */

void RBX::ModelInstance::buildJoints()
{
	RBX::JointService::singleton()->buildJoints();
}

void RBX::ModelInstance::breakJoints()
{
	/* update for jointservice */
}