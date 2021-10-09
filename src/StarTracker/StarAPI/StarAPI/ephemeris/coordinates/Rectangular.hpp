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

        std::string ToString() const;
    };
}

#endif // STARAPI_EPHEMERIS_COORDINATES_RECTANGULAR_H

