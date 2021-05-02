#pragma once

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
};