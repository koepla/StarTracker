#pragma once

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

	double julian_day_number() const {

		double real_hour = (double)(hour + minute / 60.0L + second / 3600.0L);
		int jd = (int)((int)(1461 * (int)(year + 4800 + (int)((month - 14) / 12))) / 4) + (int)((367 * (month - 2 - 12 * ((month - 14))) / 12) - 3 * (int)((year + 4900 + (int)((month - 14) / 12)) / 100)) / 4 + day - 32075;
		return jd + (real_hour / 24.0f);
	}

	double bessel_epoch() const {

		return 1900 + (julian_day_number() - 2415020.31352L) / 365.242198781L;
	}
};