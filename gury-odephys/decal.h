#ifndef DECAL_H
#define DECAL_H

#include <G3DAll.h>

#include "strings.h"
#include "instance.h"

#include "pvenums.h"
#include "content.h"

namespace RBX
{
	class PVInstance;

	namespace Render
	{
		void rawDecal(RenderDevice* d, RBX::PVInstance* pv, NormalId face, int texture, int sfactor, int dfactor);
	}

	class Decal : 
		public RBX::Instance
	{
	private:

		NormalId face;
		Content tContent;

		TextureRef texture;

		Texture::InterpolateMode interpolateMode;
		Texture::WrapMode wrapMode;

		unsigned int openGLid;
	public:
		GLenum sfactor, dfactor;

		float transparency;
		bool isDefinedSurfaceDecal; /* whether its a hardcoded engine decal or user defined decal. (should be distilled by part color) */

		TextureRef getTexture() { return texture; }
		void setTexture(TextureRef r) { texture = r; openGLid = 0; }

		void setTextureContent(Content c);
		Content getTextureContent() { return tContent; }

		NormalId getFace() { return face; }
		void setFace(NormalId _face) { face = _face; }

		void fromFile(std::string file, Texture::WrapMode wrap = Texture::TILE, Texture::InterpolateMode interpolate=Texture::BILINEAR_NO_MIPMAP);

		unsigned int getGLid() 
		{ 
			if (!openGLid && !texture.isNull()) 
				openGLid = texture->openGLID(); 
			return openGLid; 
		}

		void render(RenderDevice* rd, PVInstance* p);
		Decal() {
			openGLid = 0; 
			face = NormalId::TOP; 
			sfactor = GL_SRC_ALPHA;
			dfactor = GL_ONE_MINUS_SRC_ALPHA;
			transparency = 0.f;
			fromFile("D:\\test.png");
			setClassName("Decal");
			setName("Decal");
		}
		~Decal() { delete& texture; }
		RTTR_ENABLE(RBX::Instance);
	};
}

#endif