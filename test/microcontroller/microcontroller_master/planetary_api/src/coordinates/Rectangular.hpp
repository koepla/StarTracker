#ifndef _ASTRO_RECTANGULAR_H_
#define _ASTRO_RECTANGULAR_H_

#include <string>

namespace Astro::Coordinates {

    struct Rectangular {

        double x;
        double y;
        double z;

        Rectangular() {

            this->x = 0;
            this->y = 0;
            this->z = 0;
        }
        Rectangular(double x, double y, double z) {

            this->x = x;
            this->y = y;
            this->z = z;
        }

        std::string ToString() const {

            return "[x = " + std::to_string(x) + " | y = " + std::to_string(y) + " | z = " + std::to_string(z) + "]";
        }
    };
}

#endif // _ASTRO_RECTANGULAR_H_

