#pragma once

#include "pvinstance.h"
#include "scene.h"

namespace RBX
{
	RBX::PVInstance* getColliding(PVInstance* pvi)
	{
		Box box = Box(-pvi->getSize() * 1.1f, pvi->getSize() * 1.1f);
		Instances instances = Scene::singleton()->getArrayOfObjects();

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

	static bool colliding(PVInstance* pvi, Box box)
	{
		Instances instances = Scene::singleton()->getArrayOfObjects();

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