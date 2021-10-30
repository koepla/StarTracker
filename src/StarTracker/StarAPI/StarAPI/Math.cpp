#include "Math.hpp"

namespace StarTracker {

	double Math::Abs(double x) {

		return x < 0 ? -x : x;
	}

	double Math::Degrees(double radians) {

		return radians * 180.0 / Math::PI;
	}

	double Math::Radians(double degrees) {

		return degrees * Math::PI / 180.0;
	}

	double Math::Frac(double x) {

		return x - std::floor(x);
	}

	double Math::Mod(double a, double b) {

		return b * Math::Frac(a / b);
	}

	double Math::Sine(double angle) {

		return std::sin(Math::Radians(angle));
	}

	double Math::Cosine(double angle) {

		return std::cos(Math::Radians(angle));
	}

	double Math::Tangent(double angle) {

		return std::tan(Math::Radians(angle));
	}

	double Math::ArcSine(double angle) {

		return Math::Degrees(std::asin(angle));
	}

	double Math::ArcCosine(double angle) {

		return Math::Degrees(std::acos(angle));
	}

	double Math::ArcTangent(double angle) {

		return Math::Degrees(std::atan(angle));
	}

	double Math::ArcTangent2(double y, double x) {

		return Math::Degrees(std::atan2(y, x));
	}

	double Math::DaaToDegrees(double degree, double arcmin, double arcsec) {

		return (degree + arcmin / 60.0 + arcsec / 3600.0);
	}

	double Math::HmsToDegrees(double hour, double minute, double sec) {

		return 15.0 * (hour + minute / 60.0 + sec / 3600.0);
	}
}