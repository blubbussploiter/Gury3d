#pragma once

#include "decal.h"

namespace RBX
{
	class Texture : public RBX::Decal
	{
	public:

		Vector2 studsPerTile;

		void render(RenderDevice* rd, RBX::Render::IRenderable* p); /* render with renderFace instead of renderFaceFitForDecal */

		int getStudsPerTileU() { return studsPerTile.x; }
		int getStudsPerTileV() { return studsPerTile.y; }
		void setStudsPerTileU(int u) { studsPerTile.x = u; }
		void setStudsPerTileV(int v) { studsPerTile.y = v; }

		Texture()
		{
			wrapMode = G3D::Texture::WrapMode::TILE;
			setName("Texture");
			setClassName("Texture");
		}

		RTTR_ENABLE(RBX::Decal);

	};
}