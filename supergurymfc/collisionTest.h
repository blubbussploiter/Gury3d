#pragma once

#include "pvinstance.h"
#include "scene.h"

namespace RBX
{
	static RBX::PVInstance* getColliding(PVInstance* pvi)
	{
		Box box = Box(-pvi->getSize() * 1.1f, pvi->getSize() * 1.1f);
		Instances instances = Scene::get()->getArrayOfObjects();

		for (RBX::Instance* instance : instances)
		{
			RBX::PVInstance* pvInstance = toInstance<PVInstance>(instance);

			if (pvInstance == 0) continue;
			if (pvInstance == pvi) continue;

			Box pvBox = Box(-pvInstance->getSize() * 1.1f, pvInstance->getSize() * 1.1f);

			AABox box1, box2;
			pvBox.getBounds(box1);
			box.getBounds(box2);

			if (box1.intersects(box2))
			{
				return pvInstance;
			}
		}
		return 0;
	}

	static bool collidingIgnoringAxis(PVInstance* pvi, Box box, Vector3 normAxis)
	{
		Instances instances = Scene::get()->getArrayOfObjects();

		for (RBX::Instance* instance : instances)
		{
			RBX::PVInstance* pvInstance = toInstance<PVInstance>(instance);

			if (pvInstance == 0) continue;
			if (pvInstance == pvi) continue;

			Box pvBox = pvInstance->getBox();

			AABox box1, box2;
			pvBox.getBounds(box1);
			box.getBounds(box2);

			if (box1.intersects(box2))
			{
				Array<Vector3> contactPoints, contactNormals;
				CollisionDetection::penetrationDepthForFixedBoxFixedBox(box, pvBox, contactPoints, contactNormals);
				for (unsigned int i = 0; i < contactNormals.size(); i++)
				{
					Vector3 normal = contactNormals[i];
					if (normal == normAxis || normal == -normAxis)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	static bool colliding(PVInstance* pvi, Box box)
	{
		Instances instances = Scene::get()->getArrayOfObjects();

		for (RBX::Instance* instance : instances)
		{
			RBX::PVInstance* pvInstance = toInstance<PVInstance>(instance);

			if (pvInstance == 0) continue;
			if (pvInstance == pvi) continue;

			Box pvBox = pvInstance->getBox();

			AABox box1, box2;
			pvBox.getBounds(box1);
			box.getBounds(box2);

			if (box1.intersects(box2))
			{
				return true;
			}
		}

		return false;
	}

	static bool predict(PVInstance* pvi, CoordinateFrame futureCoord)
	{
		Box box;
		box = futureCoord.toWorldSpace(Box(-pvi->getSize(), pvi->getSize()));
		return !colliding(pvi, box);
	}
	
	static bool predictOnAxis(PVInstance* pvi, CoordinateFrame futureCoord, Vector3 axis)
	{
		Box box;
		box = futureCoord.toWorldSpace(Box(-pvi->getSize(), pvi->getSize()));
		return !collidingIgnoringAxis(pvi, box, axis);
	}

	static float getDirectionalBrickInfo(Vector3 origin, Vector3 direction, PVInstance* me, PVInstance*& firstBrickUp)
	{
		float nearest = inf();
		float top = 0.f;

		PVInstance* colliding = getColliding(me);
		if (colliding)
		{
			Array<Vector3> contactPoints, contactNormals;

			/* get two contact normals (for linkage) */

			CollisionDetection::penetrationDepthForFixedBoxFixedBox(colliding->getBox(), me->getBox(), contactPoints, contactNormals);
			if (normalize(direction) == contactNormals[0])
			{

			}
		}

		return top;
	}
}