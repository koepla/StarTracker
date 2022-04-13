#include "Math.hpp"

namespace StarTracker::Math {

	double Abs(double x) {

		return x < 0 ? -x : x;
	}

	double Degrees(double radians) {

		return radians * 180.0 / Math::PI;
	}

	double Radians(double degrees) {

		return degrees * Math::PI / 180.0;
	}

	double Frac(double x) {

		return x - std::floor(x);
	}

	double Mod(double a, double b) {

		return b * Math::Frac(a / b);
	}

	double Sine(double angle) {

		return std::sin(Math::Radians(angle));
	}

	double Cosine(double angle) {

		return std::cos(Math::Radians(angle));
	}

	double Tangent(double angle) {

		return std::tan(Math::Radians(angle));
	}

	double ArcSine(double angle) {

		return Degrees(std::asin(angle));
	}

	double ArcCosine(double angle) {

		return Degrees(std::acos(angle));
	}

	double ArcTangent(double angle) {

		return Degrees(std::atan(angle));
	}

	double ArcTangent2(double y, double x) {

		return Degrees(std::atan2(y, x));
	}

	double DaaToDegrees(double degree, double arcMinute, double arcSecond) {

		return (degree + arcMinute / 60.0 + arcSecond / 3600.0);
	}

	double HmsToDegrees(double hour, double minute, double second) {

		return 15.0 * (hour + minute / 60.0 + second / 3600.0);
	}
}