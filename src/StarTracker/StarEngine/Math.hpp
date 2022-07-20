#ifndef STARENGINE_MATH_H
#define STARENGINE_MATH_H

#include <cstdint>
#include <cmath>
#include <numbers>

namespace StarTracker::Math {

    constexpr double PI = std::numbers::pi;
    constexpr double PI2 = (2.0 * PI);
    constexpr double C0 = 299792458;
    constexpr double AU = 149597870700;

    [[nodiscard]] double Abs(double x);
    [[nodiscard]] double Degrees(double radians);
    [[nodiscard]] double Radians(double degrees);

    [[nodiscard]] double Frac(double x);
    [[nodiscard]] double Mod(double a, double b);
    [[nodiscard]] double Sine(double angle);
    [[nodiscard]] double Cosine(double angle);
    [[nodiscard]] double Tangent(double angle);
    [[nodiscard]] double ArcSine(double angle);
    [[nodiscard]] double ArcCosine(double angle);
    [[nodiscard]] double ArcTangent(double angle);
    [[nodiscard]] double ArcTangent2(double y, double x);

    [[nodiscard]] double DaaToDegrees(double degree, double arcMinute, double arcSecond);
    [[nodiscard]] double HmsToDegrees(double hour, double minute, double second);
}

#endif // STARENGINE_MATH_H