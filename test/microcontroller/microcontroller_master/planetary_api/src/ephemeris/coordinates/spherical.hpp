#pragma once

#include <string>

struct spherical {

    double r;
    double right_asc;
    double decl;

    spherical() {

        this->r = 1;
        this->right_asc = 0;
        this->decl = 0;
    }
    spherical(double right_asc, double decl, double r = 1) {

        this->right_asc = right_asc;
        this->decl = decl;
        this->r = r;
    }

    std::string to_string() {

        return "[right_ascension = " + std::to_string(right_asc) + " | declination = " + std::to_string(decl) + " | r = " + std::to_string(r) + "]";
    }
};