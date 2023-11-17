#include "classes.h"

#include "part.h"
#include "camera.h"
#include "lighting.h"
#include "workspace.h"
#include "humanoid.h"
#include "decal.h"
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
