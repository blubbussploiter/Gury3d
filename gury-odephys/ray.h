#include <G3DAll.h>
#include "workspace.h"

namespace RBX
{
	namespace World
	{
		class Ray
		{
		private:
			RBX::Instances* ignore;
			float lastIntersectionTime;
		public:
			G3D::Ray g3dRay;
			Vector3 p;
			float nearest;
			void setIgnoreList(RBX::Instances* i) { ignore = i; }
			void addIgnore(RBX::Instance* i) { ignore->push_back(i); }
			float getLastIntersectionTime() { return lastIntersectionTime; }
			RBX::ISelectable* getPartFromRay();
			Vector3 getNormalFromRay();
			Ray(G3D::Ray r)
			{
				g3dRay = r;
				ignore = new RBX::Instances();
			}
			Ray(Vector3 from, Vector3 to)
			{
				g3dRay = G3D::Ray::fromOriginAndDirection(from, to);
				ignore = new RBX::Instances();
			}
		};
		RBX::ISelectable* getPartFromG3DRay(G3D::Ray ray, Vector3& hitWorld, RBX::Instances* ignore=0);
	}
}