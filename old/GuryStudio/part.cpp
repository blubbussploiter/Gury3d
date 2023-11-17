
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

		if (getShape() == part)
		{
			auto c = getChildren();
			for (unsigned int i = 0; i < c->size(); i++)
			{
				RBX::Instance* ic = c->at(i);
				RBX::Decal* decal = dynamic_cast<RBX::Decal*>(ic);
				if (decal)
				{
					decal->render(rd, this);
				}
			}
		}
	}
	else
	{
		rd->setObjectToWorldMatrix(cframe);
		specialShape->render(rd);
	}
}