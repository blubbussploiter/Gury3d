#ifndef EXTENTS_H
#define EXTENTS_H

#include <G3DAll.h>

namespace RBX
{
	class Extents
	{
	public:
		Vector3 low, high;
		Extents() : low(Vector3::zero()), high(Vector3::zero()) {}
		Extents(Vector3 low, Vector3 high) : low(low), high(high) {}
		Vector3 size() 
		{
			return high - low;
		}
		Vector3 getCenter() 
		{
			return (low + high) * 0.5f; 
		}
		float area()
		{
			return size().x * size().z; /* i think? */
		}
		Extents toWorldSpace(const CoordinateFrame& localCoord)
		{
			Vector3 center = (low + high) * 0.5f;
			Vector3 halfSize = (high - low) * 0.5f;

			Vector3 newCenter = localCoord.pointToWorldSpace(center);
			Vector3 newHalfSize = Vector3(
				fabs(localCoord.rotation[0][0]) * halfSize[0] + fabs(localCoord.rotation[0][1]) * halfSize[1] + fabs(localCoord.rotation[0][2]) * halfSize[2],
				fabs(localCoord.rotation[1][0]) * halfSize[0] + fabs(localCoord.rotation[1][1]) * halfSize[1] + fabs(localCoord.rotation[1][2]) * halfSize[2],
				fabs(localCoord.rotation[2][0]) * halfSize[0] + fabs(localCoord.rotation[2][1]) * halfSize[1] + fabs(localCoord.rotation[2][2]) * halfSize[2]);

			return Extents(newCenter - newHalfSize, newCenter + newHalfSize);
		}

		bool separatedByMoreThan(RBX::Extents* other, float distance)
		{
			double y;
			double z;
			double x;
			double hy;
			RBX::Extents thisExpanded;

			y = this->low.y;
			z = this->low.z;
			x = this->high.x;
			hy = this->high.y;
			thisExpanded.low.y = low.x - distance;
			thisExpanded.low.z = y - distance;
			thisExpanded.high.x = z - distance;
			thisExpanded.high.y = x + distance;
			thisExpanded.high.z = hy + distance;
			return !RBX::Extents::overlapsOrTouches(other);
		}
		bool overlapsOrTouches(RBX::Extents* other)
		{
			return other->high.x >= low.x
				&& other->high.y >= low.y
				&& other->high.z >= low.z
				&& other->low.y <= high.y
				&& other->low.x <= high.x
				&& other->low.z <= high.z;
		}
	};
}

#endif