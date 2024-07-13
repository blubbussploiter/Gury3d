#ifndef PVENUMS_H
#define PVENUMS_H

#include <rttr/registration.h>

namespace G3D { class Vector3; }

namespace RBX
{
	class PVInstance;

	enum NormalId
	{
		RIGHT,
		TOP,
		BACK,
		LEFT,
		BOTTOM,
		FRONT,
		UNDEFINED
	};

	static RBX::NormalId normals[6] = {
		RBX::NormalId::FRONT,
		RBX::NormalId::BACK,
		RBX::NormalId::RIGHT,
		RBX::NormalId::LEFT,
		RBX::NormalId::TOP,
		RBX::NormalId::BOTTOM
	};

	extern G3D::Vector3 getNormalIdDirection(RBX::NormalId normal);

	extern G3D::Vector3 getNormalFromNormalId(RBX::PVInstance* object, RBX::NormalId normal);

	enum SurfaceType
	{
		Smooth,
		Glue,
		Weld,
		Studs,
		Inlet,
		UNKNOWN_FILLER,
		Hinge,
		Motor,
		SteppingMotor,
		NumSurfaces
	};

	enum Shape
	{
		Ball,
		Block,
		Cylinder
	};

	enum FormFactor
	{
		Symmetric,
		Brick,
		Plate
	};

	RTTR_REGISTRATION
	{
		rttr::registration::enumeration<NormalId>("NormalId")
			(
				rttr::value("Right", RIGHT),
				rttr::value("Left", LEFT),
				rttr::value("Top", TOP),
				rttr::value("Bottom", BOTTOM),
				rttr::value("Front", FRONT),
				rttr::value("Back", BACK)
			);

		rttr::registration::enumeration<SurfaceType>("SurfaceType")
			(
				rttr::value("Smooth", Smooth),
				rttr::value("Glue", Glue),
				rttr::value("Weld", Weld),
				rttr::value("Studs", Studs),
				rttr::value("Inlet", Inlet),
				rttr::value("Hinge", Hinge),
				rttr::value("Motor", Motor),
				rttr::value("SteppingMotor", SteppingMotor)
			);

		rttr::registration::enumeration<Shape>("Shape")
			(
				rttr::value("Ball", Ball),
				rttr::value("Block", Block),
				rttr::value("Cylinder", Cylinder)
			);

		rttr::registration::enumeration<FormFactor>("FormFactor")
			(
				rttr::value("Symmetric", Symmetric),
				rttr::value("Brick", Brick),
				rttr::value("Plate", Plate)
			);
	}
}

#endif