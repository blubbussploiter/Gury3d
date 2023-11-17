
#include "pvinstance.h"
#include "lighting.h"
#include "part.h"
#include "mesh.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<RBX::PartInstance>("Part")
		 .constructor<>()
		 .property("BrickColor", &RBX::PartInstance::getBrickColor, &RBX::PartInstance::setBrickColor);
}

void RBX::PartInstance::render(RenderDevice* rd)
{
	if (!specialShape)
	{
		PVInstance::render(rd);
	}
	else
	{
		rd->setObjectToWorldMatrix(cframe);
		specialShape->render(rd);
	}
}