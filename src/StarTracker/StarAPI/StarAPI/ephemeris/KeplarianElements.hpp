#ifndef _STAR_KEPLARIANELEMENTS_H_
#define _STAR_KEPLARIANELEMENTS_H_

namespace Star::Ephemeris {

	struct KeplarianElements {

		double SemiMajorAxis;
		double Eccentricity;
		double Inclination;
		double MeanLongitude;
		double LonPerihelion;
		double LonAscendingNode;
	};
}

#endif // _STAR_KEPLARIANELEMENTS_H_

