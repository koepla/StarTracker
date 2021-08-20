#ifndef _ASTRO_MATH_H_
#define _ASTRO_MATH_H_

#include <cmath>
#include <cstdint>

// STL Implementation will eventually be replaced by glm

namespace Astro {

    class Math {

    public:
        static inline constexpr long double PI = 3.14159265355897L;
        static inline constexpr long double PI2 = (2 * PI);

        static inline double Frac(double x) {

            return x - floor(x);
        }

        static inline double Mod(double a, double b) {

            return b * Frac(a / b);
        }

        static inline double Degrees(double radians) {
            return radians * (180.0 / PI);
        }

        static inline double Radians(double degrees) {
            return degrees * (PI / 180.0);
        }

        static inline double Sine(double angle) {
            return sin(Radians(angle));
        }

        static inline double Cosine(double angle) {
            return cos(Radians(angle));
        }

        static inline double Tangent(double angle) {
            return tan(Radians(angle));
        }

        static inline double ArcSine(double angle) {
            return Degrees(asin(angle));
        }

        static inline double ArcCosine(double angle) {
            return Degrees(acos(angle));
        }

        static inline double ArcTangent(double angle) {
            return Degrees(atan(angle));
        }

        static inline double ArcTangent2(double x, double y) {
            return Degrees(atan2(x, y));
        }

        static inline double RealDegrees(double degree, double arcmin, double arcsec) {

            return (degree + arcmin / 60.0L + arcsec / 3600.0L);
        }

        static inline double HourToDegrees(double hour, double minute, double sec) {

            return 15.0L * (hour + minute / 60.0L + sec / 3600.0L);
        }
    };
}

#endif // _ASTRO_MATH_H_