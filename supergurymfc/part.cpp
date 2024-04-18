
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

	/* render decals */

	for (unsigned int i = 0; i < children->size(); i++)
	{
		Decal* decal = toInstance<Decal>(children->at(i));
		if (!decal) continue;
		if (specialShape)
		{
			decal->render(rd, specialShape);
		}
		else
		{
			decal->render(rd, this);
		}
	}
}