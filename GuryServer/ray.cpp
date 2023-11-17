#include "ray.h"
#include "workspace.h"

#include "GuiRoot.h"

RBX::PVInstance* RBX::World::Ray::getPartFromRay()
{	
	return getPartFromG3DRay(g3dRay, p, ignore);
}

Vector3 RBX::World::Ray::getNormalFromRay()
{
	Vector3 normal = Vector3::zero();
	/*
	std::vector<RBX::Render::Renderable*> instances;
	nearest = inf();

	instances = RBX::Scene::singleton()->getArrayOfObjects();

	for (unsigned int i = 0; i < instances.size(); i++)
	{
		Box b = static_cast<RBX::PVInstance*>(instances.at(i))->getBox();

		float newdistance = g3dRay.intersectionTime(b);
		if (G3D::isFinite(newdistance) && nearest > abs(newdistance))
		{
			nearest = newdistance;
			lastIntersectionTime = newdistance;
			normal = g3dRay.origin + (g3dRay.direction * nearest);
		}
	}
	*/
	return normal;
}

RBX::PVInstance* RBX::World::getPartFromG3DRay(G3D::Ray ray, Vector3& hitWorld, RBX::Instances* ignore)
{
	/*
	RBX::Instances* instances;
	RBX::PVInstance* part = 0;
	float nearest = inf();

	instances = RBX::Workspace::singleton()->getPvs();

	for (unsigned int i = 0; i < instances->size(); i++)
	{
		RBX::PVInstance* child = static_cast<RBX::PVInstance*>(instances->at(i));

		Box b = child->getBox();

		if (ignore)
		{
			if (std::find(ignore->begin(), ignore->end(), child) != ignore->end())
			{
				continue;
			}
		}

		float newdistance = ray.intersectionTime(b);

		if (G3D::isFinite(newdistance) && nearest > abs(newdistance))
		{
			nearest = newdistance;
			hitWorld = ray.origin + (ray.direction * nearest);
			part = child;
		}
	}

	return part;
	*/
	return 0;
}
