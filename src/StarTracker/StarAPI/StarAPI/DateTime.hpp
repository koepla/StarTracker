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
		void AddYears(int64_t years) noexcept;
		void AddMonths(int64_t months) noexcept;
		void AddDays(int64_t days) noexcept;
		void AddHours(int64_t hours) noexcept;
		void AddMinutes(int64_t minutes) noexcept;
		void AddSeconds(int64_t seconds) noexcept;

	private:
		void addDays(int64_t days, const int monthDays) noexcept;

	public:
		/*
		*	returns the current date
		*/
		[[nodiscard]] static DateTime Now() noexcept;
		[[nodiscard]] static DateTime UtcNow() noexcept;
		[[nodiscard]] static int64_t UtcDiff() noexcept;

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
		*	Greenwich mean sidereal time in degrees
		*/
		[[nodiscard]] static double Gmst(const DateTime& utc) noexcept;

		/*
		*	returns the date as a formatted string
		*/
		[[nodiscard]] std::string ToString() const noexcept;
	};
}

#endif // STARAPI_DATETIME_H