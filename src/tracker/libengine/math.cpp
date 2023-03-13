#include "math.hpp"

namespace math {

    f64 Abs(f64 x) {
        return x < 0 ? -x : x;
    }

    f64 Degrees(f64 radians) {
        return radians * 180.0 / PI;
    }

    f64 Radians(f64 degrees) {
        return degrees * PI / 180.0;
    }

    f64 Fraction(f64 x) {
        return x - std::floor(x);
    }

    f64 Mod(f64 a, f64 b) {
        return b * Fraction(a / b);
    }

    f64 Sine(f64 angle) {
        return std::sin(Radians(angle));
    }

    f64 Cosine(f64 angle) {
        return std::cos(Radians(angle));
    }

    f64 Tangent(f64 angle) {
        return std::tan(Radians(angle));
    }

    f64 ArcSine(f64 x) {
        return Degrees(std::asin(x));
    }

    f64 ArcCosine(f64 x) {
        return Degrees(std::acos(x));
    }

    f64 ArcTangent(f64 x) {
        return Degrees(std::atan(x));
    }

    f64 ArcTangent2(f64 y, f64 x) {
        return Degrees(std::atan2(y, x));
    }

    f64 DaaToDegrees(f64 degree, f64 arcMinute, f64 arcSecond) {
        const auto result = std::abs(degree) + std::abs(arcMinute) / 60.0 + std::abs(arcSecond) / 3600.0;
        return degree < 0.0 ? -result : result;
    }

    f64 HmsToDegrees(f64 hour, f64 minute, f64 second) {
        return 15.0 * (hour + minute / 60.0 + second / 3600.0);
    }
}// namespace math