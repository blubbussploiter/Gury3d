#include "pvinstance.h"
#include "pvenums.h"

TextureRef RBX::getSurface(RBX::SurfaceType s)
{
	GImage surfaces;
	GImage surface;

	float y = 0;

	std::string fn = GetFileInPath("/content/textures/GurySurfaces.png");

	surfaces = GImage();
	surfaces.load(fn);

	switch (s)
	{
		case RBX::Weld:
		{
			y = 256;
			break;
		}
		case RBX::Glue:
		{
			y = 384;
			break;
		}
		case RBX::Studs:
		{
			y = 0;
			break;
		}
		case RBX::Inlet:
		{
			y = 128;
			break;
		}
	}

	bool b = surfaces.copySubImage(surface, surfaces, 0, y, 64, 128);
	Texture::Parameters params;

	params.interpolateMode = Texture::BILINEAR_MIPMAP;
	params.wrapMode = Texture::TILE;

	TextureRef r = Texture::fromGImage(fn, surface, TextureFormat::AUTO, Texture::DIM_2D, params);

	return r;
}

CoordinateFrame RBX::getSurfaceCenter(NormalId face, Vector3 size, Extents extents)
{
	CoordinateFrame center = CoordinateFrame();
	Vector3 extentsCenter, positionCenter;

	extentsCenter = extents.getCenter();

	switch (face)
	{
		case NormalId::FRONT:
		{
			positionCenter = extentsCenter + Vector3(0,0, -size.z);
			break;
		}
		case NormalId::BACK:
		{
			positionCenter = extentsCenter + Vector3(0, 0, size.z);
			break;
		}
		case NormalId::LEFT:
		{
			positionCenter = extentsCenter + Vector3(-size.x, 0, 0);
			break;
		}
		case NormalId::RIGHT:
		{
			positionCenter = extentsCenter + Vector3(size.x, 0, 0);
			break;
		}
		case NormalId::TOP:
		{
			positionCenter = extentsCenter + Vector3(0, size.y, 0);
			break;
		}
		case NormalId::BOTTOM:
		{
			positionCenter = extentsCenter + Vector3(0, -size.y, 0);
			break;
		}
	}

	CoordinateFrame lookAt;

	lookAt = extentsCenter;
	//lookAt.lookAt(positionCenter);

	return CoordinateFrame(Matrix3::identity(), positionCenter);
}