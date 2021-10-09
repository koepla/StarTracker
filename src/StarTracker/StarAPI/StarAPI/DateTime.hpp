#ifndef STARAPI_DATETIME_H
#define STARAPI_DATETIME_H

#include "Math.hpp"
#include <ctime>
#include <string>

/*
*	date struct for simplification and convenience
*/

namespace StarTracker {

	class DateTime {

	public:
		int64_t Year;
		int64_t Month;
		int64_t Day;
		int64_t Hour;
		int64_t Minute;
		int64_t Second;

		DateTime(int64_t year, int64_t month, int64_t day, int64_t hour, int64_t minute, int64_t second);

	public:

		/*
		*	returns the current date
		*/
		static DateTime Now();

		/*
		*	calculates the julian day number for a given date
		*/
		static double Jdn(const DateTime& date);
		static double Mjdn(const DateTime& date);

		/*
		*	calculates the julian centuries via the julian day number
		*/
		static double JulianCenturies(const DateTime& date, bool floor = false);

		/*
		*	returns the bessel epoch
		*/
		static double BesselEpoch(const DateTime& d);

		/*
		*	Greenwich mean sidereal time in degrees
		*/
		static double Gmst(const DateTime& d);

		/*
		*	returns the date as a formatted string
		*/
		std::string ToString() const;
	};
}

#endif //STARAPI_DATETIME_H