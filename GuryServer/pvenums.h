#ifndef PVENUMS_H
#define PVENUMS_H

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
		Unkown,
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
}

#endif