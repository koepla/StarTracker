#ifndef LIBENGINE_MATH_H
#define LIBENGINE_MATH_H

#include "utility/types.hpp"

#include <cmath>
#include <cstdint>

namespace math {

    constexpr f64 PI = 3.14159265358979323846;
    constexpr f64 PI2 = (2.0 * PI);
    constexpr f64 C0 = 299792458;
    constexpr f64 AU = 149597870700;
    constexpr f64 ARCS = 3600.0 * 180.0 / PI;

    f64 Abs(f64 x);
    f64 Degrees(f64 radians);
    f64 Radians(f64 degrees);

    f64 Fraction(f64 x);
    f64 Mod(f64 a, f64 b);
    f64 Sine(f64 angle);
    f64 Cosine(f64 angle);
    f64 Tangent(f64 angle);
    f64 ArcSine(f64 x);
    f64 ArcCosine(f64 x);
    f64 ArcTangent(f64 x);
    f64 ArcTangent2(f64 y, f64 x);

    f64 DaaToDegrees(f64 degree, f64 arcMinute, f64 arcSecond);
    f64 HmsToDegrees(f64 hour, f64 minute, f64 second);
}// namespace math

#endif// LIBENGINE_MATH_H
