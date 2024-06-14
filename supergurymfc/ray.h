
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
			IRenderableArray instances;
			RBX::ISelectable* part = 0;

			float nearest = inf();

			instances = RBX::Scene::singleton()->getArrayOfObjects();

			for (unsigned int i = 0; i < instances.size(); i++)
			{
				RBX::Instance* instance = toInstance<RBX::Instance>(instances.at(i));
				RBX::PVInstance* child = toInstance<PVInstance>(instance);

				if (child)
				{
					ISelectable::SelectableBox sb = child->getBoundingBox();
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

	}
}