#pragma once

#include <string>

namespace coordinates {

    struct rectangular {
        double x;
        double y;
        double z;

        rectangular() {

            this->x = 0;
            this->y = 0;
            this->z = 0;
        }
        rectangular(double x, double y, double z) {

            this->x = x;
            this->y = y;
            this->z = z;
        }

        std::string to_string() const {

            return "[x = " + std::to_string(x) + " | y = " + std::to_string(y) + " | z = " + std::to_string(z) + "]";
        }
    };
}

