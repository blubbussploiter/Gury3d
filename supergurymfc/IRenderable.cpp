#include "IRenderable.h"
#include "decal.h"

void RBX::Render::IRenderable::renderDecals(RenderDevice* rd, Instance* parent)
{
	if (!parent) parent = this;
	auto c = parent->getChildren();
	for (unsigned int i = 0; i < c->size(); i++)
	{
		RBX::Instance* ic = c->at(i);
		RBX::Decal* decal = toInstance<Decal>(ic);
		if (decal)
		{
			decal->render(rd, this);
		}
	}
}
