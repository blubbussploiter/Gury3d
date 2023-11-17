
#include "pvinstance.h"
#include "stdout.h"

TextureRef RBX::RenderSurfaceFactory::base_texture_weld = 0;
TextureRef RBX::RenderSurfaceFactory::base_texture_bumps = 0;
TextureRef RBX::RenderSurfaceFactory::base_texture_studs = 0;
TextureRef RBX::RenderSurfaceFactory::base_texture_inlets = 0;

unsigned int RBX::RenderSurfaceFactory::getSurfaceTexture(SurfaceType s)
{
	switch (s)
	{
	case Weld: return base_texture_weld->openGLID();
	case Glue: return base_texture_bumps->openGLID();
	case Studs: return base_texture_studs->openGLID();
	case Inlet: return base_texture_inlets->openGLID();
	default: break;
	}
	return -1;
}

void RBX::RenderSurfaceFactory::initSurfaces()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "RenderSurfaceFactory::initSurfaces()");
	base_texture_weld = RBX::getSurface(RBX::Weld);
	base_texture_bumps = RBX::getSurface(RBX::Glue);
	base_texture_studs = RBX::getSurface(RBX::Studs);
	base_texture_inlets = RBX::getSurface(RBX::Inlet);
}
