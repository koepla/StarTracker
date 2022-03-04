#ifndef STARENGINE_MATH_H
#define STARENGINE_MATH_H

#include <cstdint>
#include <cmath>
#include <numbers>

namespace StarTracker {

    class Math {

    public:
		static inline constexpr double PI = std::numbers::pi;
        static inline constexpr double PI2 = (2.0 * PI);
		static inline constexpr double C0 = 299792458;
		static inline constexpr double AU = 149597870700;

		[[nodiscard]] static double Abs(double x);
		[[nodiscard]] static double Degrees(double radians);
		[[nodiscard]] static double Radians(double degrees);

		[[nodiscard]] static double Frac(double x);
		[[nodiscard]] static double Mod(double a, double b);
		[[nodiscard]] static double Sine(double angle);
		[[nodiscard]] static double Cosine(double angle);
		[[nodiscard]] static double Tangent(double angle);
		[[nodiscard]] static double ArcSine(double angle);
		[[nodiscard]] static double ArcCosine(double angle);
		[[nodiscard]] static double ArcTangent(double angle);
		[[nodiscard]] static double ArcTangent2(double y, double x);

		[[nodiscard]] static double DaaToDegrees(double degree, double arcMinute, double arcSecond);
		[[nodiscard]] static double HmsToDegrees(double hour, double minute, double second);
	};
}

#endif // STARENGINE_MATH_H