#include "DateTime.hpp"

namespace StarTracker {

	DateTime::DateTime(int64_t year, int64_t month, int64_t day, int64_t hour, int64_t minute, int64_t second) 
	: Year(year), Month(month), Day(day), Hour(hour), Minute(minute), Second(second) 
	{

	}


	DateTime DateTime::Now() {

		std::time_t time = std::time(0);
		tm ltm = tm();
		localtime_s(&ltm, &time);

		return DateTime(static_cast<int64_t>(ltm.tm_year) + 1900, 
					static_cast<int64_t>(ltm.tm_mon) + 1, 
					static_cast<int64_t>(ltm.tm_mday),
					static_cast<int64_t>(ltm.tm_hour),
					static_cast<int64_t>(ltm.tm_min),
					static_cast<int64_t>(ltm.tm_sec));
	}

	double DateTime::Jdn(const DateTime& date) {

		return Mjdn(date) + 2400000.5L;
	}

	double DateTime::Mjdn(const DateTime& date) {

		auto mod = date;

		int64_t b;

		if (mod.Month <= 2) {

			mod.Month += 12;
			--mod.Year;
		}
		if ((10000L * mod.Year + 100L * mod.Month + mod.Day) <= 15821004L) {

			// julian calender
			b = -2 + ((mod.Year + 4716) / 4) - 1179;
		}
		else {

			// gregorian calender
			b = (mod.Year / 400) - (mod.Year / 100) + (mod.Year / 4);
		}

		double realHour = (double)(mod.Hour + mod.Minute / 60.0L + mod.Second / 3600.0L);

		return (365LL * mod.Year - 679004LL + b + int64_t(30.6001 * (mod.Month + 1LL)) + mod.Day) + realHour / 24.0L;
	}

	double DateTime::JulianCenturies(const DateTime& date, bool floor) {

		double jdn = floor ? std::floor(DateTime::Jdn(date)) : DateTime::Jdn(date);
		return (jdn - 2451545.0L) / 36525.0L;
	}

	double DateTime::BesselEpoch(const DateTime& date) {

		return 1900 + (Jdn(date) - 2415020.31352L) / 365.242198781L;
	}

	/*
	*	Greenwich mean sidereal time in degrees
	*/
	double DateTime::Gmst(const DateTime& date) {

		const double secs = 86400.0L;
		double mjd = DateTime::Mjdn(date);
		double mjd_0 = std::floor(mjd);
		double UT = secs * (mjd - mjd_0); // [s]
		double T = (mjd - 51544.5L) / 36525.0L;
		double T_0 = (mjd_0 - 51544.5L) / 36525.0L;


		double gmst = 24110.54841L + 8640184.812866 * T_0 + 1.0027379093 * UT + (0.093104 - 6.2e-6 * T) * T * T;

		double gmstDeg = Math::Degrees((Math::PI2 / secs) * Math::Mod(gmst, secs));

		if (gmstDeg > 0.0) {
			while (gmstDeg > 360.0) {

				gmstDeg -= 360.0L;
			}
		}
		else {
			while (gmstDeg < 0.0) {

				gmstDeg += 360.0L;
			}
		}

		return gmstDeg;
	}

	std::string DateTime::ToString() const {

		char buff[100];
		sprintf_s(buff, sizeof(buff),
			"[%02d-%02d-%04d | %02d:%02d:%02d]",
			static_cast<int>(Day),
			static_cast<int>(Month),
			static_cast<int>(Year),
			static_cast<int>(Hour),
			static_cast<int>(Minute),
			static_cast<int>(Second));

		return std::string(buff);
	}
}