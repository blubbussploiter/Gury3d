#include "classes.h"

#include "part.h"
#include "camera.h"
#include "lighting.h"
#include "workspace.h"
#include "humanoid.h"
#include "basescript.h"
#include "hopperbin.h"
#include "decal.h"
#include "texture.h"
#include "model.h"
#include "mesh.h"

RBX::Instance* RBX::fromName(std::string className)
{
	/* replace with a template, or something */

	if (className == "Part")
	{
		return new RBX::PartInstance();
	}

	if (className == "Model")
	{
		return new RBX::ModelInstance();
	}

	if (className == "SpecialMesh")
	{
		return new RBX::Render::SpecialMesh();
	}

	if (className == "Humanoid")
	{
		return new RBX::Humanoid();
	}

	if (className == "Decal")
	{
		return new RBX::Decal();
	}

	if (className == "Texture")
	{
		return new RBX::Texture();
	}

	if (className == "Script" ||
		className == "BaseScript")
	{
		return new RBX::BaseScript();
	}

	if (className == "HopperBin")
	{
		return new RBX::HopperBin();
	}

	if (className == "Lighting")
	{
		return RBX::Lighting::singleton();
	}

	if (className == "Camera")
	{
		return RBX::Camera::singleton();
	}

	if (className == "Workspace" ||
		className == "PVWorkspace")
	{
		return RBX::Workspace::singleton();
	}

	return 0;
}
