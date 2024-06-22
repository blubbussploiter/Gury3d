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
	for (Instance* object : sceneObjects)
	{
		object->remove();
	}
	sceneObjects.clear();
}

RBX::Instances RBX::Scene::getArrayOfObjects()
{
	return sceneObjects;
}

bool opaqueRule(RBX::Render::Renderable* r)
{
	return (!r->transparency && !r->renderedLast);
}

bool transparentRule(RBX::Render::Renderable* r)
{
	return (!r->renderedLast && r->transparency > 0);
}

void render3DSurface(RBX::Render::Renderable* r, RenderDevice* rd)
{
	r->render3DSurfaces(rd);
}

void RBX::Scene::updateSteppables()
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		ISteppable* steppableObject = toInstance<ISteppable>(sceneObjects.at(i));
		if (steppableObject)
		{
			steppableObject->onStep();
		}
	}
}

void RBX::Scene::updateSteppablesKernelly()
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		ISteppable* steppableObject = toInstance<ISteppable>(sceneObjects.at(i));
		if (steppableObject)
		{
			steppableObject->onKernelStep();
		}
	}
}

void RBX::Scene::opaquePass(RenderDevice* rd)
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		Render::Renderable* iRenderable = toInstance<Render::Renderable>(sceneObjects.at(i));
		if (iRenderable && opaqueRule(iRenderable) &&
			!RBX::IsA<Render::SpecialMesh>(iRenderable))
		{
			iRenderable->render(rd);
		}
	}
}

void RBX::Scene::reflectancePass(RenderDevice* rd) /* keep working on this sometime idkf */
{
	rd->pushState();
	rd->disableLighting();

	//rd->configureReflectionMap(0, getGlobalSky()->getEnvironmentMap());
	//rd->setDepthWrite(0);

	//iterate(rd, reflectanceRule, renderRenderable);
	
	rd->popState();
}

void RBX::Scene::transparentPass(RenderDevice* rd)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		Render::Renderable* iRenderable = toInstance<Render::Renderable>(sceneObjects.at(i));
		if (iRenderable &&
			transparentRule(iRenderable) &&
			!RBX::IsA<Render::SpecialMesh>(iRenderable))
		{
			iRenderable->render(rd);
		}
	}

	glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_BLEND);
}

void RBX::Scene::darkPass(RenderDevice* rd)
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		Render::Renderable* iRenderable = toInstance<Render::Renderable>(sceneObjects.at(i));
		if (iRenderable && 
			opaqueRule(iRenderable) &&
			!RBX::IsA<Render::SpecialMesh>(iRenderable))
		{
			iRenderable->render3DSurfaces(rd);
		}
	}

	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		Render::Renderable* iRenderable = toInstance<Render::Renderable>(sceneObjects.at(i));
		if (iRenderable && 
			opaqueRule(iRenderable) 
			&& iRenderable->unaffectedByLight &&
			!RBX::IsA<Render::SpecialMesh>(iRenderable))
		{
			iRenderable->render(rd);
		}
	}
}

void RBX::Scene::lastPass(RenderDevice* rd)
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		Render::Renderable* iRenderable = toInstance<Render::Renderable>(sceneObjects.at(i));
		if (iRenderable && iRenderable->renderedLast &&
			!RBX::IsA<Render::SpecialMesh>(iRenderable))
		{
			iRenderable->render(rd);
		}
	}
}

void RBX::Scene::initializeKernel()
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		PVInstance* pvInstance = toInstance<PVInstance>(sceneObjects.at(i));
		if (pvInstance)
		{
			pvInstance->initializeForKernel();
		}
	}
}

void RBX::Scene::saveStartPVs() /* before run: save each position of everything in the scene */
{
	for (unsigned int i = 0; i < sceneObjects.size(); i++)
	{
		PVInstance* pvInstance = toInstance<PVInstance>(sceneObjects.at(i));
		if (pvInstance)
		{
			pvInstance->savePV();
		}
	}
}

void RBX::Scene::onWorkspaceDescendentAdded(Render::Renderable* descendent)
{
	RBX::Instance* instance = (Instance*)descendent;
	if (IsA<Render::Renderable>(instance) || IsA<RBX::ISteppable>(instance))
	{
		Render::SpecialMesh* specialMesh = toInstance<Render::SpecialMesh>(descendent);
		PVInstance* pvInstance = toInstance<PVInstance>(descendent);

		if (specialMesh)
		{
			Render::Renderable* p = toInstance<Render::Renderable>(descendent->getParent());
			if (p)
			{
				p->specialShape = descendent;
			}
		}

		if (pvInstance)
		{
			Kernel::get()->addQueuedPrimitive(pvInstance->primitive);
		}

		sceneObjects.push_back(descendent);
	}
}

void RBX::Scene::onWorkspaceDescendentRemoved(RBX::Render::Renderable* descendent)
{
	if (std::find(
		sceneObjects.begin(),
		sceneObjects.end(),
		descendent) != sceneObjects.end())
	{
		sceneObjects.erase(std::remove(
			sceneObjects.begin(),
			sceneObjects.end(),
			descendent));
	}
}
