#include "view.h"

#include "scene.h"
#include "lighting.h"
#include "players.h"

#include "datamodel.h"
#include "selection.h"

#include "diagnosticsWorldDrawer.h"
#include "StudioTool.h"

RBX::View* rbx_view;

void RBX::View::onWorkspaceDescendentAdded(RBX::Instance* descendent)
{
	RBX::Scene::singleton()->onWorkspaceDescendentAdded((RBX::Render::Renderable*)descendent);
}

void RBX::View::onWorkspaceDescendentRemoved(RBX::Instance* descendent)
{
	RBX::Scene::singleton()->onWorkspaceDescendentRemoved((RBX::Render::Renderable*)descendent);
}

void RBX::View::presetLighting()
{
	getEffectSettings()->applyEffects(0, params, lighting);
}

void RBX::View::turnOnLights(RenderDevice* device)
{
	int n = 1;
	Color3 ambientColor;

	ambientColor = (lighting->ambientBottom + lighting->ambientTop) / 2.0f;

	device->setSpecularCoefficient(1);
	device->setColorClearValue(colorClearValue);

	for (int i = 0; i < lighting->lightArray.size(); i++)
	{
		device->setLight(n++, lighting->lightArray[i]);
	}

	for (int i = 0; i < lighting->shadowedLightArray.size(); i++)
	{
		device->setLight(n++, lighting->shadowedLightArray[i]);
	}

	device->setAmbientLightColor(ambientColor);

	if (effectSettings->_hemisphereLighting)
	{
		if (lighting->ambientBottom != ambientColor)
		{
			Color3 ambient = lighting->ambientBottom - ambientColor;
			device->setLight(n++, GLight::directional(Vector3(0.4f, -1.0f, 0.1f), ambient, 0, 1));
		}

		if (lighting->ambientTop != ambientColor)
		{
			Color3 ambient = lighting->ambientTop - ambientColor;
			device->setLight(n++, GLight::directional(Vector3::unitY(), ambient, 0, 1));
		}
	}
}

void RBX::View::renderScene(RenderDevice* rd)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);

	rd->enableLighting();
	rd->setPolygonOffset(-0.02);

	rd->setLight(0, GLight::directional(params.lightDirection, params.lightColor * 0.9f, 1, 1));

	turnOnLights(rd);

	RBX::Scene::singleton()->opaquePass(rd);
	RBX::Scene::singleton()->transparentPass(rd);

	rd->disableLighting();

	if (!effectSettings->toneMap.isNull())
	{
		effectSettings->toneMap->endFrame(rd);
	}

	RBX::Scene::singleton()->darkPass(rd);
	RBX::Scene::singleton()->lastPass(rd);

	rd->setPolygonOffset(0.02);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glCullFace(GL_NONE);

}

void RBX::View::oneFrame(RenderDevice* renderDevice, Camera* projection, SkyRef sky)
{
	Datamodel* datamodel = RBX::Datamodel::getDatamodel();

	presetLighting();

	renderDevice->beginFrame();
	//shadows->generateShadowMap(renderDevice);

	renderDevice->clear();
	renderDevice->setProjectionAndCameraMatrix(*projection->getCamera());

	renderDevice->setStencilConstant(0);
	renderDevice->setStencilClearValue(0);
	renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);

	if (!sky.isNull())
	{
		sky->render(params);
	}

	renderDevice->pushState();
	//shadows->render(renderDevice);

	Selection::renderSelection(renderDevice);

	if (RBX::Studio::current_Tool)
	{
		RBX::Studio::current_Tool->doGraphics(renderDevice);
	}

	renderScene(renderDevice);

	renderDevice->popState();

	if (!sky.isNull())
	{
		sky->renderLensFlare(params);
	}

	renderDevice->push2D();

	datamodel->guiRoot->render(renderDevice);
	datamodel->message->render(renderDevice);

	Selection::renderDragBox(renderDevice);

	renderDevice->pop2D();

	renderDevice->endFrame();

}

RBX::View* RBX::View::singleton()
{
	if (!rbx_view)
	{
		rbx_view = new RBX::View();
	}
	return rbx_view;
}
