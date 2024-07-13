#include "shadows.h"
#include "view.h"

void RBX::Shadows::configureLights(LightingParameters lighting)
{
	lightProjectionMatrix = Matrix4::orthogonalProjection(-lightProjX, lightProjX, -lightProjY, lightProjY, lightProjNear, lightProjFar);
	lightCFrame.lookAt(-lighting.lightDirection, Vector3::unitY());
	lightCFrame.translation = lighting.lightDirection * 20;
	lightMVP = lightProjectionMatrix * lightCFrame.inverse();
}

void RBX::Shadows::generateShadowMap(RenderDevice* renderDevice)
{
	renderDevice->clear(false, true, false);

	Rect2D rect = Rect2D::xywh(0, 0, shadowMapSize, shadowMapSize);

	renderDevice->pushState();
	renderDevice->setViewport(rect);
	renderDevice->setProjectionMatrix(Matrix4::orthogonalProjection(-lightProjX, lightProjX, -lightProjY, lightProjY, lightProjNear, lightProjFar));

	renderDevice->setCameraToWorldMatrix(lightCFrame);
	renderDevice->disableColorWrite();

	renderDevice->setPolygonOffset(4);

	RBX::View::get()->renderScene(renderDevice);

	renderDevice->popState();

	shadowMap->copyFromScreen(rect);

}

void RBX::Shadows::render(RenderDevice* renderDevice)
{
	if (GLCaps::supports_GL_ARB_shadow()) {
		renderDevice->configureShadowMap(1, lightMVP, shadowMap);
	}
}
