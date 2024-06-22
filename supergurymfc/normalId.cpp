
#include <G3DAll.h>

#include "pvenums.h"
#include "pvinstance.h"

Vector3 RBX::getNormalIdDirection(RBX::NormalId normal)
{
	switch (normal)
	{
	case RBX::NormalId::TOP:
	{
		return Vector3::unitY();
	}
	case RBX::NormalId::BOTTOM:
	{
		return -Vector3::unitY();
	}
	case RBX::NormalId::RIGHT:
	{
		return Vector3::unitX();
	}
	case RBX::NormalId::LEFT:
	{
		return -Vector3::unitX();
	}
	case RBX::NormalId::FRONT:
	{
		return Vector3::unitZ();
	}
	case RBX::NormalId::BACK:
	{
		return -Vector3::unitZ();
	}
	}
	return Vector3::zero();
}

Vector3 RBX::getNormalFromNormalId(RBX::PVInstance* object, RBX::NormalId normal)
{
	return object->getCoordinateFrame().vectorToWorldSpace(getNormalIdDirection(normal));
}