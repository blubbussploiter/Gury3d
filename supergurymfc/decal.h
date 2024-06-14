#ifndef DECAL_H
#define DECAL_H

#include <G3DAll.h>

#include "strings.h"
#include "instance.h"

#include "content.h"
#include "IRenderable.h"

namespace RBX
{
	class PVInstance;

	namespace Render
	{
		void rawDecal(RenderDevice* d, RBX::Render::Renderable* pv, NormalId face, int texture, bool repeat=1);
	}

	class Decal : 
		public RBX::Instance
	{
	private:

		Content tContent;

		Texture::InterpolateMode interpolateMode;
		Texture::WrapMode wrapMode;

	public:

		NormalId face;
		TextureRef texture;

		float transparency;
		bool isDefinedSurfaceDecal; /* whether its a hardcoded engine decal or user defined decal. (should be distilled by part color) */

		TextureRef getTexture() { return texture; }
		void setTexture(TextureRef r) { texture = r; }

		void setTextureContent(Content c);
		Content getTextureContent() { return tContent; }

		NormalId getFace() { return face; }
		void setFace(NormalId _face) { face = _face; }

		void fromFile(std::string file, Texture::WrapMode wrap = Texture::TILE, Texture::InterpolateMode interpolate=Texture::NEAREST_MIPMAP);

		unsigned int getGLid() 
		{ 
			if (!texture.isNull()) 
				return texture->openGLID(); 
			return 0;
		}

		virtual void render(RenderDevice* rd, RBX::Render::Renderable* p);
		void initContentTexture();

		Decal() {
			face = NormalId::TOP;
			transparency = 0.f;
			setClassName("Decal");
			setName("Decal");
		}
		~Decal() { delete& texture; }
		RTTR_ENABLE(RBX::Instance);
	};
}

#endif