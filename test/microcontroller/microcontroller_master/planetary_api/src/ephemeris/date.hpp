#pragma once

#include "math.hpp"
#include <ctime>

class date {

public:

	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	date(int year, int month, int day, int hour, int minute, int second) {
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
	static date now() {

		std::time_t time = std::time(0);
		tm* ltm = new tm();
		localtime_s(ltm, &time);

		date d = date(ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
		
		delete ltm;
		return d;
	}

	/*
	*	calculates the julian day number for a given date
	*/
	static double jdn(const date& d) {

		return mjdn(d) + 2400000.5L;
	}

	static double mjdn(const date& d) {

		auto mod = d;

		int b;

		if (mod.month <= 2) {
			
			mod.month += 12;
			--mod.year;
		}
		if ((10000L * mod.year + 100L * mod.month + mod.day) <= 15821004L) {

			// julian calender
			b = -2 + ((mod.year + 4716) / 4) - 1179;
		}
		else {

			//gregorian calender
			b = (mod.year / 400) - (mod.year / 100) + (mod.year / 4);
		}

		double real_hour = (double)(mod.hour + mod.minute / 60.0L + mod.second / 3600.0L);

		return (365L * mod.year - 679004L + b + int(30.6001 * (mod.month + 1)) + mod.day) + real_hour/24.0L;
		
	}

	/*
	*	calculates the julian centuries via the julian day number
	*/
	static double julian_centuries(const date& d, bool floor = false) {

		double jdn = floor ? std::floor(date::jdn(d)) : date::jdn(d);
		return (jdn - 2451545.0L) / 36525.0L;
	}

	/*
	*	returns the bessel epoch
	*/
	static double bessel_epoch(const date& d) {

		return 1900 + (jdn(d) - 2415020.31352L) / 365.242198781L;
	}

	/*
	*	Greenwich mean sidereal time in degrees
	*/
	static double gmst(const date& d) {

		const double secs = 86400.0L;
		double mjd	= date::mjdn(d);
		double mjd_0	= std::floor(mjd);
		double UT		= secs * (mjd - mjd_0); // [s]
		double T		= (mjd - 51544.5L) / 36525.0L;
		double T_0	= (mjd_0 - 51544.5L) / 36525.0L;

		
		double gmst = 24110.54841L + 8640184.812866 * T_0 + 1.0027379093 * UT + (0.093104 - 6.2e-6 * T) * T * T;

		double gmst_deg = radians2deg((PI2 / secs) * mod(gmst, secs));

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
	*	returns a nicely formatted string
	*/
	std::string to_string() const {

		char buff[100];
		sprintf_s(buff, sizeof(buff), "[%02d-%02d-%04d | %02d:%02d:%02d]", day, month, year, hour, minute, second);
		
		return std::string(buff);
	}
};