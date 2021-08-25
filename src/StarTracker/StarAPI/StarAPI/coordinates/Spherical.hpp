#ifndef _STARAPI_SPHERICAL_H_
#define _STARAPI_SPHERICAL_H_

#include <string>

namespace Star::Coordinates {

    struct Spherical {

        double Radius;
        double RightAscension;
        double Declination;

        Spherical();
        Spherical(double rightAscension, double declination, double radius = 1);

        std::string ToString() const;
    };
}

#endif // _STARAPI_SPHERICAL_H_