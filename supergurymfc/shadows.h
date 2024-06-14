#pragma once

#include <G3DAll.h>

namespace RBX
{
	class Shadows
	{
	private:

		int shadowMapSize;
		double lightProjX, lightProjY, lightProjNear, lightProjFar;

		CoordinateFrame lightCFrame;
		TextureRef shadowMap;
		Matrix4 lightMVP;
		Matrix4 lightProjectionMatrix;

	public:
		void configureLights(LightingParameters lighting);
		void generateShadowMap(RenderDevice* renderDevice);
		void render(RenderDevice* renderDevice);
		Shadows()
		{
			lightProjX = 17;
			lightProjY = 17;
			lightProjNear = 1;
			lightProjFar = 40;
			shadowMapSize = 512;
			shadowMap = Texture::createEmpty("Shadow Map", shadowMapSize, shadowMapSize, TextureFormat::RGB8, Texture::DIM_2D_NPOT);
		}
	};
}