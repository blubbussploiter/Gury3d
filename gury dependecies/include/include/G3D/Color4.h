/**
 @file Color4.h
 
 Color class
 
 @maintainer Morgan McGuire, matrix@graphics3d.com
 @cite Portions based on Dave Eberly's Magic Software Library
      at <A HREF="http://www.magic-software.com">http://www.magic-software.com</A>
 
 @created 2002-06-25
 @edited  2006-01-16

 Copyright 2000-2006, Morgan McGuire.
 All rights reserved.
 */

#ifndef G3D_COLOR4_H
#define G3D_COLOR4_H

#include "G3D/platform.h"
#include "G3D/g3dmath.h"
#include "G3D/Color3.h"
#include <string>

namespace G3D {

/**
 Do not subclass-- this implementation makes assumptions about the
 memory layout.
 */
class Color4 {
private:
    // Hidden operators
    bool operator<(const Color4&) const;
    bool operator>(const Color4&) const;
    bool operator<=(const Color4&) const;
    bool operator>=(const Color4&) const;

public:

    /**
     * Does not initialize fields.
     */
    Color4 ();

    Color4(const Color3& c3, float a = 1.0);

    Color4(const class Color4uint8& c);

    Color4(class BinaryInput& bi);

    Color4(const class Vector4& v);

    /**
     * Initialize from G3D::Reals.
     */
    Color4(float r, float g, float b, float a = 1.0);
    
    /**
     * Initialize from array of G3D::Reals.
     */
    Color4(float value[4]);

    /**
     * Initialize from another color.
     */
    Color4(const Color4& other);

    void serialize(class BinaryOutput& bo) const;
    void deserialize(class BinaryInput& bi);

    /**
     Initialize from an HTML-style color (e.g. 0xFFFF0000 == RED)
     */
    static Color4 fromARGB(uint32);

    /**
     * Channel values.
     */
    float r, g, b, a;

    // access vector V as V[0] = V.r, V[1] = V.g, V[2] = V.b, v[3] = V.a
    //
    // WARNING.  These member functions rely on
    // (1) Color4 not having virtual functions
    // (2) the data packed in a 3*sizeof(float) memory block
    float& operator[] (int i) const;
    operator float* ();
    operator const float* () const;

    // assignment and comparison
    Color4& operator= (const Color4& rkVector);
    bool operator== (const Color4& rkVector) const;
    bool operator!= (const Color4& rkVector) const;
    unsigned int hashCode() const;

    // arithmetic operations
    Color4 operator+ (const Color4& rkVector) const;
    Color4 operator- (const Color4& rkVector) const;
    Color4 operator* (float fScalar) const;
    Color4 operator/ (float fScalar) const;
    Color4 operator- () const;
    friend Color4 operator* (double fScalar, const Color4& rkVector);

    // arithmetic updates
    Color4& operator+= (const Color4& rkVector);
    Color4& operator-= (const Color4& rkVector);
    Color4& operator*= (float fScalar);
    Color4& operator/= (float fScalar);

    bool fuzzyEq(const Color4& other) const;
    bool fuzzyNe(const Color4& other) const;

    std::string toString() const;

    inline Color4 max(const Color4& other) const {
        return Color4(G3D::max(r, other.r), G3D::max(g, other.g), G3D::max(b, other.b), G3D::max(a, other.a));
    }

    inline Color4 min(const Color4& other) const {
        return Color4(G3D::min(r, other.r), G3D::min(g, other.g), G3D::min(b, other.b), G3D::min(a, other.a));
    }

    /** r + g + b + a */
    inline float sum() const {
        return r + g + b + a;
    }

    // Special values.
    // Intentionally not inlined: see Matrix3::identity() for details.
    static const Color4& zero();
    static const Color4& clear();

    static const Color4& inf();

    // Deprecated. See Matrix3::identity() for details.
    /** @deprecated Use Color4::zero() */
    static const Color4 ZERO;
    /** @deprecated Use Color4::clear() */
    static const Color4 CLEAR;
};

/**
 Extends the c3 with alpha = 1.0
 */
Color4 operator*(const Color3& c3, const Color4& c4);

} // namespace

#include "Color4.inl"

#endif
