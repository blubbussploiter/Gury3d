#include "view.h"

#include "scene.h"
#include "lighting.h"
#include "players.h"

#include "datamodel.h"
#include "selection.h"

#include "jointsservice.h"

#include "diagnosticsWorldDrawer.h"
#include "studio/StudioTool.h"

RBX::View* rbx_view;

void RBX::View::onWorkspaceDescendentAdded(RBX::Instance* descendent)
{
	RBX::Scene::get()->onWorkspaceDescendentAdded((RBX::Render::IRenderable*)descendent);
}

void RBX::View::onWorkspaceDescendentRemoved(RBX::Instance* descendent)
{
	RBX::Scene::get()->onWorkspaceDescendentRemoved((RBX::Render::IRenderable*)descendent);
}

void RBX::View::presetLighting()
{
	if (!readyForToneMap)
	{
		params = getEffectSettings()->toneMap->prepareLightingParameters(params);
		lighting = getEffectSettings()->toneMap->prepareLighting(lighting);
		readyForToneMap = true;
	}
}

void RBX::View::turnOnLights(RenderDevice* device)
{
	int n = 1;
	Color3 ambientColor;

	presetLighting();

	ambientColor = (lighting->ambientBottom + lighting->ambientTop) / 2.2f;

	device->setSpecularCoefficient(1);
	device->setColorClearValue(colorClearValue);
	
	device->setLight(0, GLight::directional(params.lightDirection, params.lightColor*0.9f, 1, 1));

	for (int i = 0; i < lighting->lightArray.size(); i++)
	{
		device->setLight(n++, lighting->lightArray[i]);
	}

	for (int i = 0; i < lighting->shadowedLightArray.size(); i++)
	{
		device->setLight(n++, lighting->shadowedLightArray[i]);
	}

	device->setAmbientLightColor(ambientColor);

	if (getEffectSettings()->_hemisphereLighting)
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

	turnOnLights(rd);

	RBX::Scene::get()->opaquePass(rd);
	RBX::Scene::get()->transparentPass(rd);

	rd->disableLighting();

	RBX::Scene::get()->darkPass(rd);
	RBX::Scene::get()->lastPass(rd);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glCullFace(GL_NONE);
}

void RBX::View::oneFrame(RenderDevice* renderDevice, Camera* projection, SkyRef sky)
{
	Datamodel* datamodel = RBX::Datamodel::getDatamodel();

	//presetLighting();

	renderDevice->beginFrame();

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

	Selection::get()->renderSelection(renderDevice);

	if (RBX::Studio::current_Tool)
	{
		RBX::Studio::current_Tool->doGraphics(renderDevice);
	}

	renderScene(renderDevice);

	RBX::Diagnostics::get_Renderer()->render(renderDevice);

	renderDevice->popState();

	if (!effectSettings->toneMap.isNull())
	{
		effectSettings->toneMap->endFrame(renderDevice);
	}

	if (!sky.isNull())
	{
		sky->renderLensFlare(params);
	}

	renderDevice->push2D();

	datamodel->guiRoot->render(renderDevice);
	datamodel->message->render(renderDevice);

	Selection::get()->renderDragBox(renderDevice);
	Diagnostics::get_Renderer()->render2D(renderDevice);

	Mouse::getMouse()->render(renderDevice);

	renderDevice->pop2D();

	renderDevice->endFrame();

}

RBX::View* RBX::View::get()
{
	if (!rbx_view)
	{
		rbx_view = new RBX::View();
	}
	return rbx_view;
}
