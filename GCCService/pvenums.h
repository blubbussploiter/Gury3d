#ifndef PVENUMS_H
#define PVENUMS_H

#include <rttr/registration.h>

namespace RBX
{
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

	enum SurfaceType
	{
		Smooth,
		Glue,
		Weld,
		Studs,
		Inlet,
		Hinge,
		Motor,
		SteppingMotor,
		NumSurfaces
	};

	enum Shape
	{
		ball,
		part,
		cylinder
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
				rttr::value("ball", ball),
				rttr::value("block", part),
				rttr::value("cylinder", cylinder)
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