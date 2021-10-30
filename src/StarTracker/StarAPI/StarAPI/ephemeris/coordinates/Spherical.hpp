#ifndef STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H
#define STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

    struct Spherical {

        double Radius;
        double RightAscension;
        double Declination;

        Spherical();
        Spherical(double rightAscension, double declination, double radius = 1);

        /**
        * Formats member variables to a std::string
        *
        * @return the member variables `Radius`, `RightAscension` and `Declination`
        *     formatted as std::string
        *
        */
        [[nodiscard]] std::string ToString() const noexcept;
    };
}

#endif // STARAPI_EPHEMERIS_COORDINATES_SPHERICAL_H