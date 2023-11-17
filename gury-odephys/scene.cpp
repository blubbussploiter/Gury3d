#include "scene.h"

#include "pvinstance.h"
#include "datamodel.h"

RBX::Scene* RBX::Scene::singleton()
{
	return RBX::Datamodel::getDatamodel()->scene;
}

void RBX::Scene::close()
{
	renderObjects.clear();
}

std::vector<RBX::Render::Renderable*> RBX::Scene::getArrayOfObjects()
{
	return renderObjects;
}

bool opaqueRule(RBX::Render::Renderable* r)
{
	return (r->isRenderable && r->transparency == 0 && !r->renderedLast);
}

bool transparentRule(RBX::Render::Renderable* r)
{
	return (r->isRenderable 
		&& r->transparency > 0 && r->transparency < 1
		&& !r->renderedLast);
}

bool lastRenderRule(RBX::Render::Renderable* r)
{
	return r->renderedLast;
}

bool physicsRule(RBX::Render::Renderable* r)
{
	return r->isAffectedByPhysics;
}

bool steppableRule(RBX::Render::Renderable* r)
{
	RBX::ISteppable* steppable;
	RBX::Instance* i = dynamic_cast<RBX::Instance*>(r);
	steppable = dynamic_cast<RBX::ISteppable*>(i);
	return steppable;
}

void renderRenderable(RBX::Render::Renderable* r, RenderDevice* rd)
{
	r->render(rd);
}

void render3DSurface(RBX::Render::Renderable* r, RenderDevice* rd)
{
	r->render3DSurfaces(rd);
}

void update3DObject(RBX::Render::Renderable* r, RenderDevice* rd)
{
	RBX::PVInstance* pv = dynamic_cast<RBX::PVInstance*>(r);
	if (pv && pv->body)
	{
		RBX::RunService::singleton()->getPhysics()->createBody(pv);
	}
}

void stepStepper(RBX::Render::Renderable* r, RenderDevice* rd)
{
	RBX::ISteppable* steppable;
	RBX::Instance* i = dynamic_cast<RBX::Instance*>(r);
	steppable = dynamic_cast<RBX::ISteppable*>(i);
	if (steppable)
	{
		steppable->onStep();
	}
}

void RBX::Scene::baseRender(RenderDevice* rd, bool(*rule)(RBX::Render::Renderable*), void(*render)(RBX::Render::Renderable*, RenderDevice*))
{
	for (unsigned int i = 0; i < renderObjects.size(); i++)
	{
		RBX::Render::Renderable* r = renderObjects.at(i);
		if (rule(r))
		{
			render(r, rd);
		}
	}
}

void RBX::Scene::updateSteppables()
{
	baseRender(0, steppableRule, stepStepper);
}

void RBX::Scene::updatePhysicsObjects()
{
	baseRender(0, physicsRule, update3DObject);
}

void RBX::Scene::opaquePass(RenderDevice* rd)
{
	baseRender(rd, opaqueRule, renderRenderable);
}

void RBX::Scene::transparentPass(RenderDevice* rd)
{
	baseRender(rd, transparentRule, renderRenderable);
}

void RBX::Scene::darkPass(RenderDevice* rd)
{
	baseRender(rd, opaqueRule, render3DSurface);
}

void RBX::Scene::lastPass(RenderDevice* rd)
{
	baseRender(rd, lastRenderRule, renderRenderable);
}

void RBX::Scene::onWorkspaceDescendentAdded(RBX::Render::Renderable* descendent)
{
	if (descendent->isRenderable
		|| steppableRule(descendent))
	{

		RBX::Render::Renderable* r = dynamic_cast<RBX::Render::Renderable*>(descendent);

		if (r->isSpecialShape)
		{
			RBX::Render::Renderable* p = (RBX::Render::Renderable*)r->getParent();
			if (p->isRenderable)
			{
				p->specialShape = r;
			}
		}

		renderObjects.push_back(r);
	}
}

void RBX::Scene::onWorkspaceDescendentRemoved(RBX::Render::Renderable* descendent)
{
	if (std::find(
		renderObjects.begin(), 
		renderObjects.end(), 
		descendent) != renderObjects.end())
	{
		renderObjects.erase(std::remove(
			renderObjects.begin(), 
			renderObjects.end(),
			descendent));
	}
}
