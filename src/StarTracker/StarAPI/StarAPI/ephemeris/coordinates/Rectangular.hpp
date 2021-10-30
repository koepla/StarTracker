#ifndef STARAPI_EPHEMERIS_COORDINATES_RECTANGULAR_H
#define STARAPI_EPHEMERIS_COORDINATES_RECTANGULAR_H

#include <string>

namespace StarTracker::Ephemeris::Coordinates {

    struct Rectangular {

        double X;
        double Y;
        double Z;

        Rectangular();
        Rectangular(double x, double y, double z);

        /**
        * Formats member variables to a std::string
        *
        * @return the member variables `X`, `Y` and `Z`
        *     formatted as std::string
        *
        */
        [[nodiscard]] std::string ToString() const noexcept;
    };
}

#endif // STARAPI_EPHEMERIS_COORDINATES_RECTANGULAR_H

