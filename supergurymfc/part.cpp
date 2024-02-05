
#include "lighting.h"
#include "part.h"
#include "mesh.h"

#include "stdout.h"

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
		rd->setObjectToWorldMatrix(pv->position);
		rd->setShininess(25.0f);

		rd->setColor(Color4(color, alpha));

		specialShape->render(rd);

		//rd->setColor(Color4(1,1,1, 1)); /* revert */
		//specialShape->renderDecals(rd, this);
	}
}