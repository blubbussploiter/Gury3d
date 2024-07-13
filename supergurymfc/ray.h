
#pragma once

#include <vector>
#include <G3DAll.h>
#include "workspace.h"

namespace RBX
{
	namespace World
	{
		class Ray
		{
		private:
			Instances empty;
			float lastIntersectionTime;
		public:
			G3D::Ray g3dRay;
			Vector3 p;
			float nearest;

			Ray(G3D::Ray r)
			{
				g3dRay = r;
			}
			Ray(Vector3 from, Vector3 to)
			{
				g3dRay = G3D::Ray::fromOriginAndDirection(from, to);
			}
		};

		static Instances empty_ignoreList = {}; /* default argument for function below */

		template <typename IgnoredItem>
		inline RBX::ISelectable* getPartFromG3DRay(G3D::Ray ray, Vector3& hitWorld, bool ignoreNonCollidableObjects=false, std::vector<IgnoredItem*>& ignore = empty_ignoreList)
		{
			Instances instances;
			RBX::ISelectable* part = 0;

			float nearest = inf();

			instances = RBX::Scene::get()->getArrayOfObjects();

			for (unsigned int i = 0; i < instances.size(); i++)
			{
				RBX::Instance* instance = instances.at(i);
				RBX::PVInstance* child = toInstance<PVInstance>(instance);

				if (child)
				{
					Render::Geometry sb = child->getInstanceGeometry();
					Box b;

					b = child->getCFrame().toWorldSpace(Box(-sb.size, sb.size));

					if (std::find(ignore.begin(), ignore.end(), child) != ignore.end()
						|| (ignoreNonCollidableObjects && !child->canCollide))
					{
						continue;
					}

					float newdistance = ray.intersectionTime(b);

					if (G3D::isFinite(newdistance) && nearest > abs(newdistance))
					{
						nearest = newdistance;
						hitWorld = ray.origin + (ray.direction * nearest);
						part = child;
					}
				}
			}

			return part;

		}

		template <typename IgnoredItem>
		Vector3 getHitFromRay(G3D::Ray ray, bool ignoreNonCollidableObjects = false, std::vector<IgnoredItem*>& ignore = empty_ignoreList)
		{
			Vector3 hit = Vector3::zero();
			getPartFromG3DRay<IgnoredItem>(ray, hit, ignoreNonCollidableObjects, ignore);
			return hit;
		}


		template <typename IgnoredItem>
		PVInstance* getPVInstanceFromRay(G3D::Ray ray, Vector3& hitWorld, bool ignoreNonCollidableObjects = false, std::vector<IgnoredItem*>& ignore = empty_ignoreList)
		{
			ISelectable* i = getPartFromG3DRay<IgnoredItem>(ray, hitWorld, ignoreNonCollidableObjects, ignore);
			Instance* instance = dynamic_cast<Instance*>(i);
			return toInstance<PVInstance>(instance);
		}

	}
}