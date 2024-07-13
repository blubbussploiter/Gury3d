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
	CoordinateFrame center;
	Vector3 extentsCenter;

	extentsCenter = extents.getCenter();

	switch (face)
	{
		case NormalId::FRONT:
		{
			center = extentsCenter + Vector3(0,0, -size.z);
			center.rotation = Matrix3::fromAxisAngle(Vector3::unitY(), toRadians(90));
			break;
		}
		case NormalId::BACK:
		{
			center = extentsCenter + Vector3(0, 0, size.z);
			center.rotation = Matrix3::fromAxisAngle(Vector3::unitY(), toRadians(90));
			break;
		}
		case NormalId::LEFT:
		{
			center = extentsCenter + Vector3(-size.x, 0, 0);
			break;
		}
		case NormalId::RIGHT:
		{
			center = extentsCenter + Vector3(size.x, 0, 0);
			break;
		}
		case NormalId::TOP:
		{
			center = extentsCenter + Vector3(0, size.y, 0);
			center.rotation = Matrix3::fromAxisAngle(Vector3::unitZ(), toRadians(90));
			break;
		}
		case NormalId::BOTTOM:
		{
			center = extentsCenter + Vector3(0, -size.y, 0);
			center.rotation = Matrix3::fromAxisAngle(Vector3::unitZ(), toRadians(90));
			break;
		}
	}

	return center;
}