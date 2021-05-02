#pragma once

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
};