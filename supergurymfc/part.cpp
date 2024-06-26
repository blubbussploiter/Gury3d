
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
		specialShape->render(rd);
	}
	renderDecals(rd);
}

void RBX::PartInstance::renderDecals(RenderDevice* rd)
{
	/* render decals */

	for (unsigned int i = 0; i < children->size(); i++)
	{
		Decal* decal = toInstance<Decal>(children->at(i));

		if (!decal) continue;

		rd->setObjectToWorldMatrix(pv->position);
		rd->setColor(Color4(1, 1, 1));

		if (specialShape)
		{
			specialShape->renderDecal(rd, decal);
		}
		else
		{
			renderDecal(rd, decal);
		}
	}
}
