#ifndef RBX_MATH_H
#define RBX_MATH_H
#include <G3DAll.h>
#include "pvinstance.h"

namespace RBX
{
    namespace Math
    {
        /* i ripped all of these from IDA */
        static double angleToE0(const G3D::Vector2 v)
        {
            double result; // st7
            G3D::Vector2 u; // [esp+0h] [ebp-Ch] BYREF
            float y; // [esp+8h] [ebp-4h]

            u.y = v.x;
            y = v.y;
            u.unitize(0.000001f);
            result = acos(u.y);
            if (y < 0.0)
                return 6.2831855 - result;
            return result;
        }
        static double radWrap(float rad)
        {
            double result; // st7
            float v2; // [esp+Ch] [ebp-4h]

            result = rad;
            if (rad < -3.1415927 || result >= 3.1415927)
            {
                v2 = floor((result + 3.1415927) * 0.15915495);
                return rad - (double)(int)v2 * 6.283185;
            }
            return result;
        }
        static RBX::NormalId getClosestObjectNormalId(const G3D::Vector3* worldV, const G3D::Matrix3* objectR)
        {
            long double v2; // st7
            long double v3; // st6
            long double v4; // st5
            long double v5; // st3
            long double v6; // st2
            long double v7; // st4
            RBX::NormalId result; // eax
            long double v9; // st7

            v2 = objectR->elt[1][0] * worldV->y + objectR->elt[2][0] * worldV->z + worldV->x * objectR->elt[0][0];
            v3 = objectR->elt[1][1] * worldV->y + objectR->elt[0][1] * worldV->x + objectR->elt[2][1] * worldV->z;
            v4 = objectR->elt[1][2] * worldV->y + objectR->elt[0][2] * worldV->x + objectR->elt[2][2] * worldV->z;
            v5 = fabs(v3);
            v6 = fabs(v2);
            v7 = fabs(v4);
            result = RBX::NormalId::UNDEFINED;
            if (v6 > v5)
            {
                if (v7 < v6)
                {
                    if (v2 <= 0.0)
                        return RBX::LEFT;
                    else
                        return RBX::RIGHT;
                }
                v9 = v4;
                goto LABEL_10;
            }
            v9 = v4;
            if (v7 >= v5)
            {
            LABEL_10:
                result = RBX::BACK;
                if (v9 <= 0.0)
                    return RBX::FRONT;
                return result;
            }
            result = RBX::TOP;
            if (v3 <= 0.0)
                return RBX::BOTTOM;
            return result;
        }
    }
}
#endif