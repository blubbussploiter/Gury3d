/**
 @file Capsule.h
  
 @maintainer Morgan McGuire, matrix@graphics3d.com
  
 @created 2003-02-07
 @edited  2005-08-20

 Copyright 2000-2006, Morgan McGuire.
 All rights reserved.
 */

#ifndef G3D_CAPSULE_H
#define G3D_CAPSULE_H

#include "G3D/platform.h"
#include "G3D/g3dmath.h"
#include "G3D/Vector3.h"

namespace G3D {

class Line;
class AABox;
/**
 A shape formed by extruding a sphere along a line segment.
 */
class Capsule {
private:
	Vector3			p1;
	Vector3			p2;

public:

    /** @deprecated To be replaced with a (float) radius accessor of the same name.*/
	double			radius;

    /** Uninitialized */
    Capsule();
    Capsule(class BinaryInput& b);
	Capsule(const Vector3& _p1, const Vector3& _p2, double _r);
	void serialize(class BinaryOutput& b) const;
	void deserialize(class BinaryInput& b);
	
	/** The line down the center of the capsule */
	Line getAxis() const;

	Vector3 getPoint1() const;

	Vector3 getPoint2() const;

    /** Distance between the sphere centers.  The total extent of the cylinder is 
        2r + h. */
    inline float height() const {
        return (p1 - p2).magnitude();
    }

    inline Vector3 center() const {
        return (p1 + p2) / 2.0;
    }

    /** Get a reference frame in which the center of mass is the origin and Y is the axis of the capsule.*/
    void getReferenceFrame(class CoordinateFrame& cframe) const;

    /**
     Returns true if the point is inside the capsule or on its surface.
     */
    bool contains(const Vector3& p) const;

    /** @deprecated */
	float getRadius() const;

    /** @deprecated Use volume() */
	float getVolume() const;

    inline float volume() const {
        return getVolume();
    }

    /** @deprecated */
	float getSurfaceArea() const;

    inline float area() const {
        return getSurfaceArea();
    }

    /** Get axis aligned bounding box */
    void getBounds(AABox& out) const;

    /** Random world space point with outward facing normal. */
    void getRandomSurfacePoint(Vector3& P, Vector3& N) const;

    /** Point selected uniformly at random over the volume. */
    Vector3 randomInteriorPoint() const;
};

} // namespace

#endif
