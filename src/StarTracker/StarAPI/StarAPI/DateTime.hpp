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

		DateTime(int64_t year, int64_t month, int64_t day, int64_t hour, int64_t minute, int64_t second) noexcept;

	public:

		/*
		*	returns the current date
		*/
		[[nodiscard]] static DateTime Now() noexcept;

		/*
		*	calculates the julian day number for a given date
		*/
		[[nodiscard]] static double Jdn(const DateTime& date) noexcept;
		[[nodiscard]] static double Mjdn(const DateTime& date) noexcept;

		/*
		*	calculates the julian centuries via the julian day number
		*/
		[[nodiscard]] static double JulianCenturies(const DateTime& date, bool floor = false) noexcept;

		/*
		*	returns the bessel epoch
		*/
		[[nodiscard]] static double BesselEpoch(const DateTime& d) noexcept;

		/*
		*	Local mean sidereal time in degrees
		* 
		*	Caution: This may still be an issue, it works for now
		*/

#pragma message ("Temporary fix for Gmst, because we would only be allowed to pass the relative UTC time. This may still produce some unknown issues.") 
		[[nodiscard]] static double Lmst(const DateTime& utc) noexcept;

		/*
		*	returns the date as a formatted string
		*/
		[[nodiscard]] std::string ToString() const noexcept;
	};
}

#endif // STARAPI_DATETIME_H