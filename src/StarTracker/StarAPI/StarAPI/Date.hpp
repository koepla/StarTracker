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
		int64_t year;
		int64_t month;
		int64_t day;
		int64_t hour;
		int64_t minute;
		int64_t second;

		Date(int year, int month, int day, int hour, int minute, int second) {

			this->year = year;
			this->month = month;
			this->day = day;
			this->hour = hour;
			this->minute = minute;
			this->second = second;
		}

	public:

		/*
		*	returns the current date
		*/
		static Date Now() {

			std::time_t time = std::time(0);
			tm* ltm = new tm();
			localtime_s(ltm, &time);

			Date d = Date(ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

			delete ltm;
			return d;
		}

		/*
		*	calculates the julian day number for a given date
		*/
		static double Jdn(const Date& d) {

			return Mjdn(d) + 2400000.5L;
		}

		static double Mjdn(const Date& d) {

			auto mod = d;

			int64_t b;

			if (mod.month <= 2) {

				mod.month += 12;
				--mod.year;
			}
			if ((10000L * mod.year + 100L * mod.month + mod.day) <= 15821004L) {

				// julian calender
				b = -2 + ((mod.year + 4716) / 4) - 1179;
			}
			else {

				// gregorian calender
				b = (mod.year / 400) - (mod.year / 100) + (mod.year / 4);
			}

			double real_hour = (double)(mod.hour + mod.minute / 60.0L + mod.second / 3600.0L);

			return (365LL * mod.year - 679004LL + b + int64_t(30.6001 * (mod.month + 1LL)) + mod.day) + real_hour / 24.0L;

		}

		/*
		*	calculates the julian centuries via the julian day number
		*/
		static double JulienCenturies(const Date& d, bool floor = false) {

			double jdn = floor ? std::floor(Date::Jdn(d)) : Date::Jdn(d);
			return (jdn - 2451545.0L) / 36525.0L;
		}

		/*
		*	returns the bessel epoch
		*/
		static double BesselEpoch(const Date& d) {

			return 1900 + (Jdn(d) - 2415020.31352L) / 365.242198781L;
		}

		/*
		*	Greenwich mean sidereal time in degrees
		*/
		static double Gmst(const Date& d) {

			const double secs = 86400.0L;
			double mjd = Date::Mjdn(d);
			double mjd_0 = std::floor(mjd);
			double UT = secs * (mjd - mjd_0); // [s]
			double T = (mjd - 51544.5L) / 36525.0L;
			double T_0 = (mjd_0 - 51544.5L) / 36525.0L;


			double gmst = 24110.54841L + 8640184.812866 * T_0 + 1.0027379093 * UT + (0.093104 - 6.2e-6 * T) * T * T;

			double gmst_deg = Math::Degrees((Math::PI2 / secs) * Math::Mod(gmst, secs));

			if (gmst_deg > 0.0) {
				while (gmst_deg > 360.0) {

					gmst_deg -= 360.0L;
				}
			}
			else {
				while (gmst_deg < 0.0) {

					gmst_deg += 360.0L;
				}
			}

			return gmst_deg;
		}

		/*
		*	returns the date as a formatted string
		*/
		std::string ToString() const {

			char buff[100];
			sprintf_s(buff, sizeof(buff), 
				"[%02d-%02d-%04d | %02d:%02d:%02d]", 
				static_cast<int>(day), 
				static_cast<int>(month), 
				static_cast<int>(year), 
				static_cast<int>(hour), 
				static_cast<int>(minute), 
				static_cast<int>(second));

			return std::string(buff);
		}
	};
}

#endif //_STARAPI_DATE_H_