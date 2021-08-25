#ifndef _STARAPI_DATE_H_
#define _STARAPI_DATE_H_

#include "Math.hpp"
#include <ctime>
#include <string>

/*
*	date struct for simplification and convenience
*/

namespace Star {

	class Date {

	public:
		int64_t Year;
		int64_t Month;
		int64_t Day;
		int64_t Hour;
		int64_t Minute;
		int64_t Second;

		Date(int64_t year, int64_t month, int64_t day, int64_t hour, int64_t minute, int64_t second);

	public:

		/*
		*	returns the current date
		*/
		static Date Now();

		/*
		*	calculates the julian day number for a given date
		*/
		static double Jdn(const Date& date);
		static double Mjdn(const Date& date);

		/*
		*	calculates the julian centuries via the julian day number
		*/
		static double JulienCenturies(const Date& date, bool floor = false);

		/*
		*	returns the bessel epoch
		*/
		static double BesselEpoch(const Date& d);

		/*
		*	Greenwich mean sidereal time in degrees
		*/
		static double Gmst(const Date& d);

		/*
		*	returns the date as a formatted string
		*/
		std::string ToString() const;
	};
}

#endif //_STARAPI_DATE_H_