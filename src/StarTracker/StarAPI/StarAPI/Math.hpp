#ifndef STARAPI_MATH_H
#define STARAPI_MATH_H

#include <cstdint>
#include <cmath>
#include <numbers>

namespace StarTracker {

    class Math {

    public:
		static inline constexpr double PI = std::numbers::pi;
        static inline constexpr double PI2 = (2.0 * PI);

		static double Abs(double x);
		static double Frac(double x);
		static double Mod(double a, double b);
		static double Degrees(double radians);
		static double Radians(double degrees);
		static double Sine(double angle);
		static double Cosine(double angle);
		static double Tangent(double angle);
		static double ArcSine(double angle);
		static double ArcCosine(double angle);
		static double ArcTangent(double angle);
		static double ArcTangent2(double y, double x);
		static double DaaToDegrees(double degree, double arcmin, double arcsec);
		static double HmsToDegrees(double hour, double minute, double sec);
    };
}

#endif // STARAPI_MATH_H