#ifndef _STARAPI_SPHERICAL_H_
#define _STARAPI_SPHERICAL_H_

#include <string>

namespace Star::Coordinates {

    struct Spherical {

        double radius;
        double rightAscension;
        double declination;

        Spherical() {

            this->radius = 1;
            this->rightAscension = 0;
            this->declination = 0;
        }
        Spherical(double right_asc, double decl, double r = 1) {

            this->rightAscension = right_asc;
            this->declination = decl;
            this->radius = r;
        }

        std::string ToString() {

            return "[right_ascension = " + std::to_string(rightAscension) + " | declination = " + std::to_string(declination) + " | r = " + std::to_string(radius) + "]";
        }
    };
}

#endif // _STARAPI_SPHERICAL_H_