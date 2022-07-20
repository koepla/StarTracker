#ifndef STARENGINE_EPHEMERIS_KEPLERIANELEMENTS_H
#define STARENGINE_EPHEMERIS_KEPLERIANELEMENTS_H

namespace StarTracker::Ephemeris {

    struct KeplerianElements {

        double SemiMajorAxis;
        double Eccentricity;
        double Inclination;
        double MeanLongitude;
        double LonPerihelion;
        double LonAscendingNode;
    };
}

#endif // STARENGINE_EPHEMERIS_KEPLERIANELEMENTS_H

