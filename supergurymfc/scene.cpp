#include "scene.h"

#include "pvinstance.h"
#include "datamodel.h"

#include "stdout.h"
#include "mesh.h"

#include "appmanager.h"

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
	return (!r->transparency && !r->renderedLast);
}

bool reflectanceRule(RBX::Render::Renderable* r)
{
	return (r->reflectance > 0);
}

bool transparentRule(RBX::Render::Renderable* r)
{
	return (!r->renderedLast && r->transparency > 0);
}

bool lastRenderRule(RBX::Render::Renderable* r)
{
	return r->renderedLast && !r->transparency;
}

bool physicsRule(RBX::Render::Renderable* r)
{
	return RBX::IsA<RBX::PVInstance>(r);
}

bool darkRule(RBX::Render::Renderable* r)
{
	return opaqueRule(r) && r->unaffectedByLight;
}

bool steppableRule(RBX::Render::Renderable* r)
{
	RBX::ISteppable* steppable;
	RBX::Instance* i = dynamic_cast<RBX::Instance*>(r);
	steppable = dynamic_cast<RBX::ISteppable*>(i);
	return steppable;
}

bool isPvInstance(RBX::Render::Renderable* r)
{
	RBX::Instance* i = dynamic_cast<RBX::Instance*>(r);
	return RBX::IsA<RBX::PVInstance>(i);
}

void renderRenderable(RBX::Render::Renderable* r, RenderDevice* rd)
{
	r->render(rd);
}

void render3DSurface(RBX::Render::Renderable* r, RenderDevice* rd)
{
	r->render3DSurfaces(rd);
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

void initializePVInstance(RBX::Render::Renderable* r, RenderDevice* rd)
{
	RBX::PVInstance* pvInstance = dynamic_cast<RBX::PVInstance*>(r);
	if (pvInstance)
	{
		pvInstance->initializeForKernel();
	}
}

void savePVPosition(RBX::Render::Renderable* r, RenderDevice* rd) /* in future, maybe save each value for propertydescriptor, go back and change them back */
{
	RBX::PVInstance* pvInstance = dynamic_cast<RBX::PVInstance*>(r);
	if (pvInstance)
	{
		pvInstance->savePV();
	}

}

void RBX::Scene::iterate(RenderDevice* rd, bool(*rule)(RBX::Render::Renderable*), void(*render)(RBX::Render::Renderable*, RenderDevice*))
{
	for (unsigned int i = 0; i < renderObjects.size(); i++)
	{
		RBX::Render::Renderable* r = renderObjects.at(i);
		if (rule(r) && !RBX::IsA<RBX::Render::SpecialMesh>(r))
		{
			render(r, rd);
		}
	}
}

void RBX::Scene::updateSteppables()
{
	iterate(0, steppableRule, stepStepper);
}

void RBX::Scene::opaquePass(RenderDevice* rd)
{
	iterate(rd, opaqueRule, renderRenderable);
}

void RBX::Scene::reflectancePass(RenderDevice* rd) /* keep working on this sometime idkf */
{
	rd->pushState();
	rd->disableLighting();

	rd->configureReflectionMap(0, getGlobalSky()->getEnvironmentMap());
	rd->setDepthWrite(0);

	iterate(rd, reflectanceRule, renderRenderable);
	
	rd->popState();

}

void RBX::Scene::transparentPass(RenderDevice* rd)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	iterate(rd, transparentRule, renderRenderable);

	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_BLEND);
}

void RBX::Scene::darkPass(RenderDevice* rd)
{
	iterate(rd, opaqueRule, render3DSurface);
	iterate(rd, darkRule, renderRenderable);
}

void RBX::Scene::lastPass(RenderDevice* rd)
{
	iterate(rd, lastRenderRule, renderRenderable);
}

void RBX::Scene::initializeKernel()
{
	iterate(0, isPvInstance, initializePVInstance);
}

void RBX::Scene::saveStartPVs() /* before run: save each position of everything in the scene */
{
	iterate(0, isPvInstance, savePVPosition);
}

void RBX::Scene::onWorkspaceDescendentAdded(RBX::Render::Renderable* descendent)
{
	if (RBX::IsA<RBX::Render::Renderable>(descendent) || steppableRule(descendent))
	{
		RBX::Render::Renderable* r = toInstance<RBX::Render::Renderable>(descendent);

		if (RBX::IsA<RBX::Render::SpecialMesh>(r))
		{
			RBX::Render::Renderable* p = toInstance<RBX::Render::Renderable>(r->getParent());
			if (p)
			{
				p->specialShape = r;
			}
		}

		if (RBX::IsA<PVInstance>(r))
		{
			if (RunService::singleton()->isRunning)
			{
				toInstance<PVInstance>(r)->initializeForKernel();
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
