#ifndef STARAPI_EPHEMERIS_KEPLERIANELEMENTS_H
#define STARAPI_EPHEMERIS_KEPLERIANELEMENTS_H

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

#endif // STARAPI_EPHEMERIS_KEPLERIANELEMENTS_H

