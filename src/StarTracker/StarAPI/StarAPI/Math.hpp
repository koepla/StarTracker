#ifndef _STARAPI_MATH_H_
#define _STARAPI_MATH_H_

#include <glm/glm.hpp>
#include <cstdint>

// STL Implementation will eventually be replaced by glm

namespace Star {

    class Math {

    public:
        static inline constexpr long double PI = 3.14159265355897L;
        static inline constexpr long double PI2 = (2 * PI);

        static inline double Frac(double x) {

            return x - glm::floor(x);
        }

        static inline double Mod(double a, double b) {

            return b * Frac(a / b);
        }

        static inline double Degrees(double radians) {

            return glm::degrees(radians);
        }

        static inline double Radians(double degrees) {

            return glm::radians(degrees);
        }

        static inline double Sine(double angle) {

            return glm::sin(glm::radians(angle));
        }

        static inline double Cosine(double angle) {

            return glm::cos(glm::radians(angle));
        }

        static inline double Tangent(double angle) {

            return glm::tan(glm::radians(angle));
        }

        static inline double ArcSine(double angle) {

            return glm::degrees(glm::asin(angle));
        }

        static inline double ArcCosine(double angle) {

            return glm::degrees(glm::acos(angle));
        }

        static inline double ArcTangent(double angle) {

            return glm::degrees(glm::atan(angle));
        }

        static inline double ArcTangent2(double x, double y) {

            return glm::degrees(atan2(x, y));
        }

        static inline double RealDegrees(double degree, double arcmin, double arcsec) {

            return (degree + arcmin / 60.0L + arcsec / 3600.0L);
        }

        static inline double HourToDegrees(double hour, double minute, double sec) {

            return 15.0L * (hour + minute / 60.0L + sec / 3600.0L);
        }
    };
}

#endif // _STARAPI_MATH_