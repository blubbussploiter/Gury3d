#include "view.h"

#include "datamodel.h"

#include "lighting.h"
#include "players.h"

#include "selection.h"

RBX::View* rbx_view;

void RBX::View::onWorkspaceDescendentAdded(RBX::Instance* descendent)
{
	RBX::Scene::singleton()->onWorkspaceDescendentAdded((RBX::Render::Renderable*)descendent);
}

void RBX::View::onWorkspaceDescendentRemoved(RBX::Instance* descendent)
{
	RBX::Scene::singleton()->onWorkspaceDescendentRemoved((RBX::Render::Renderable*)descendent);
}

void RBX::View::render(RenderDevice* rd)
{

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	RBX::Lighting::singleton()->begin(rd);

	rd->setPolygonOffset(-0.2f);

	RBX::Scene::singleton()->opaquePass(rd);
	RBX::Scene::singleton()->transparentPass(rd);	
	RBX::Scene::singleton()->lastPass(rd);

	RBX::Lighting::singleton()->end(rd);

	RBX::Scene::singleton()->darkPass(rd);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

}

void RBX::View::oneFrame(RenderDevice* renderDevice, Camera* projection, SkyRef sky)
{
	Datamodel* datamodel = RBX::Datamodel::getDatamodel();
	LightingParameters params;

	params = datamodel->lighting->getParameters();

	renderDevice->beginFrame();

	renderDevice->clear();
	renderDevice->setProjectionAndCameraMatrix(*projection->getCamera());

	renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);

	if (!sky.isNull())
	{
		sky->render(params);
	}
	
	RBX::Selection::renderSelection(renderDevice);
	render(renderDevice);

	RBX::Network::getPlayers()->updatePlayerList();

	renderDevice->push2D();
	datamodel->guiRoot->render(renderDevice);
	renderDevice->pop2D();

	if (!sky.isNull())
	{
		sky->renderLensFlare(params);
	}

	renderDevice->endFrame();

}

RBX::View* RBX::View::singleton()
{
	if (!rbx_view) rbx_view = new RBX::View();
	return rbx_view;
}
