
#include "pvinstance.h"
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

void RBX::PartInstance::renderDecals(RenderDevice* rd, Render::Renderable* base)
{
	auto c = getChildren();
	for (unsigned int i = 0; i < c->size(); i++)
	{
		RBX::Instance* ic = c->at(i);
		RBX::Decal* decal = dynamic_cast<RBX::Decal*>(ic);
		if (decal)
		{
			decal->render(rd, base);
		}
	}
}

void RBX::PartInstance::render(RenderDevice* rd)
{
	if (!specialShape)
	{
		PVInstance::render(rd);
		if (shape == part) renderDecals(rd, this);
	}
	else
	{
		rd->setObjectToWorldMatrix(cframe);
		renderDecals(rd, specialShape);
		glColor(color.r, color.g, color.b, alpha);
		specialShape->render(rd);
	}
}