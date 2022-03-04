#ifndef STARENGINE_DATETIME_H
#define STARENGINE_DATETIME_H

#include <StarEngine/Math.hpp>

#include <ctime>
#include <string>

/*
*	date struct for simplification and convenience
*/

namespace StarTracker {

	class DateTime {

	public:
		std::int64_t Year;
		std::int64_t Month;
		std::int64_t Day;
		std::int64_t Hour;
		std::int64_t Minute;
		std::int64_t Second;

		DateTime(std::int64_t year, std::int64_t month, std::int64_t day, std::int64_t hour, std::int64_t minute, std::int64_t second) noexcept;

	public:
		void AddYears(std::int64_t years) noexcept;
		void AddMonths(std::int64_t months) noexcept;
		void AddDays(std::int64_t days) noexcept;
		void AddHours(std::int64_t hours) noexcept;
		void AddMinutes(std::int64_t minutes) noexcept;
		void AddSeconds(std::int64_t seconds) noexcept;

	private:
		void addDays(std::int64_t days, const std::int32_t monthDays) noexcept;

	public:
		/*
		*	returns the current date
		*/
		[[nodiscard]] static DateTime Now() noexcept;
		[[nodiscard]] static DateTime UtcNow() noexcept;
		[[nodiscard]] static std::int64_t UtcDiff() noexcept;

		/*
		*	calculates the julian day number for a given date
		*/
		[[nodiscard]] static double Jdn(DateTime date) noexcept;
		[[nodiscard]] static double Mjdn(DateTime date) noexcept;

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

#endif // STARENGINE_DATETIME_H