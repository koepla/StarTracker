#ifndef _STARAPI_RECTANGULAR_H_
#define _STARAPI_RECTANGULAR_H_

#include <string>

namespace Star::Coordinates {

    struct Rectangular {

        double X;
        double Y;
        double Z;

        Rectangular();
        Rectangular(double x, double y, double z);

        std::string ToString() const;
    };
}

#endif // _STARAPI_RECTANGULAR_H_

