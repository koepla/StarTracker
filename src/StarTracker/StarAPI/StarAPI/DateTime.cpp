#include "DateTime.hpp"

namespace StarTracker {

	DateTime::DateTime(std::int64_t year, std::int64_t month, std::int64_t day, std::int64_t hour, std::int64_t minute, std::int64_t second) noexcept
		: Year{ year }, Month{ month }, Day{ day }, Hour{ hour }, Minute{ minute }, Second{ second } {

	}

	void DateTime::AddYears(std::int64_t years) noexcept {
	
		Year += years;
	}

	void DateTime::AddMonths(std::int64_t months) noexcept {

		if (months > 0) {

			if (Month + months > 12) {

				AddYears(1);
				AddMonths(months - 12);
			}
			else {

				Month += months;
			}
		}
		else if (months < 0) {

			if (Month + months < 1) {

				AddYears(-1);
				AddMonths(months + 12);
			}
			else {

				Month += months;
			}
		}
	}

	void DateTime::AddDays(std::int64_t days) noexcept {

		switch (Month) {

		case 1:  addDays(days, 31); break;
		case 2: {

			if (Year % 4 == 0) {

				if (Year % 100 == 0) {

					if (Year % 400 == 0) {

						addDays(days, 29);
					}
					else {

						addDays(days, 28);
					}
				}
				else {

					addDays(days, 29);
				}
			}
			else {

				addDays(days, 28);
			}

		} break;
		case 3:  addDays(days, 31); break;
		case 4:  addDays(days, 30); break;
		case 5:  addDays(days, 31); break;
		case 6:  addDays(days, 30); break;
		case 7:  addDays(days, 31); break;
		case 8:  addDays(days, 31); break;
		case 9:  addDays(days, 30); break;
		case 10: addDays(days, 31); break;
		case 11: addDays(days, 30); break;
		case 12: addDays(days, 31); break;
		default: break;
		}
	}

	void DateTime::AddHours(std::int64_t hours) noexcept {

		if (hours > 0) {

			if (Hour + hours > 23) {

				AddDays(1);
				AddHours(hours - 24);
			}
			else {

				Hour += hours;
			}
		}
		else if (hours < 0) {

			if (Hour + hours < 0) {

				AddDays(-1);
				AddHours(hours + 24);
			}
			else {

				Hour += hours;
			}
		}
	}

	void DateTime::AddMinutes(std::int64_t minutes) noexcept {

		if (minutes > 0) {

			if (Minute + minutes > 59) {

				AddHours(1);
				AddMinutes(minutes - 60);
			}
			else {

				Minute += minutes;
			}
		}
		else if (minutes < 0) {

			if (Minute + minutes < 0) {

				AddHours(-1);
				AddMinutes(minutes + 60);
			}
			else {

				Minute += minutes;
			}
		}
	}

	void DateTime::AddSeconds(std::int64_t seconds) noexcept {

		if (seconds > 0) {

			if (Second + seconds > 59) {

				AddMinutes(1);
				AddSeconds(seconds - 60);
			}
			else {

				Second += seconds;
			}
		}
		else if (seconds < 0) {

			if (Second + seconds < 0) {

				AddMinutes(-1);
				AddSeconds(seconds + 60);
			}
			else {

				Second += seconds;
			}
		}
	}

	void DateTime::addDays(std::int64_t days, const std::int32_t monthDays) noexcept {

		if (days > 0) {

			if (Day + days > monthDays) {

				AddMonths(1);
				AddDays(days - monthDays);
			}
			else {

				Day += days;
			}
		}
		else if (days < 0) {

			if (Day + days < 1) {

				AddMonths(-1);
				AddDays(days + monthDays);
			}
			else {

				Day += days;
			}
		}
	}

	DateTime DateTime::Now() noexcept {

		const auto time = std::time(0);

		tm ltm{};
		localtime_s(&ltm, &time);

		return DateTime{ 

			static_cast<std::int64_t>(ltm.tm_year) + 1900,
			static_cast<std::int64_t>(ltm.tm_mon) + 1,
			static_cast<std::int64_t>(ltm.tm_mday),
			static_cast<std::int64_t>(ltm.tm_hour),
			static_cast<std::int64_t>(ltm.tm_min),
			static_cast<std::int64_t>(ltm.tm_sec) 
		};
	}

	DateTime DateTime::UtcNow() noexcept
	{
		const auto time = std::time(0);

		tm ltm{};
		gmtime_s(&ltm, &time);

		return DateTime{

			static_cast<std::int64_t>(ltm.tm_year) + 1900,
			static_cast<std::int64_t>(ltm.tm_mon) + 1,
			static_cast<std::int64_t>(ltm.tm_mday),
			static_cast<std::int64_t>(ltm.tm_hour),
			static_cast<std::int64_t>(ltm.tm_min),
			static_cast<std::int64_t>(ltm.tm_sec)
		};
	}

	std::int64_t DateTime::UtcDiff() noexcept {

		return DateTime::Now().Hour - DateTime::UtcNow().Hour;
	}

	double DateTime::Jdn(DateTime date) noexcept {

		return Mjdn(date) + 2400000.5;
	}

	double DateTime::Mjdn(DateTime date) noexcept {

		auto b = std::int64_t{ 0 };

		if (date.Month <= 2) {

			date.Month += 12;
			--date.Year;
		}
		if ((10000 * date.Year + 100 * date.Month + date.Day) <= 15821004) {

			// julian calender
			b = -2 + ((date.Year + 4716) / 4) - 1179;
		}
		else {

			// gregorian calender
			b = (date.Year / 400) - (date.Year / 100) + (date.Year / 4);
		}

		const auto realHour = static_cast<double>(date.Hour + date.Minute / 60.0 + date.Second / 3600.0);

		return (365 * date.Year - 679004 + b + static_cast<std::int64_t>( 30.6001 * (date.Month + 1LL)) + date.Day) + realHour / 24.0;
	}

	double DateTime::JulianCenturies(const DateTime& date, bool floor) noexcept {

		const auto jdn = floor ? std::floor(DateTime::Jdn(date)) : DateTime::Jdn(date);
		return (jdn - 2451545.0) / 36525.0;
	}

	double DateTime::BesselEpoch(const DateTime& date) noexcept {

		return 1900.0 + (Jdn(date) - 2415020.31352) / 365.242198781;
	}

	/*
	*	Greenwhich mean sidereal time in degrees
	*/
	double DateTime::Gmst(const DateTime& date) noexcept {

		const auto secondsInDay = 86400.0;
		const auto meanJulianDayNumber = DateTime::Mjdn(date);
		const auto meanJulianDayNumberFloor = std::floor(meanJulianDayNumber);
		const auto UT = secondsInDay * (meanJulianDayNumber - meanJulianDayNumberFloor);
		const auto T = (meanJulianDayNumber - 51544.5) / 36525.0;
		const auto T_0 = (meanJulianDayNumberFloor - 51544.5) / 36525.0;

		const auto greenwhichMeanSiderealTime = 24110.54841 + 8640184.812866 * T_0 + 1.0027379093 * UT + (0.093104 - 6.2e-6 * T) * T * T;
		auto greenwhichMeanSiderealTimeDegrees = Math::Degrees((Math::PI2 / secondsInDay) * Math::Mod(greenwhichMeanSiderealTime, secondsInDay));

		if (greenwhichMeanSiderealTimeDegrees > 0.0) {

			while (greenwhichMeanSiderealTimeDegrees > 360.0) {

				greenwhichMeanSiderealTimeDegrees -= 360.0;
			}
		}
		else {

			while (greenwhichMeanSiderealTimeDegrees < 0.0) {

				greenwhichMeanSiderealTimeDegrees += 360.0;
			}
		}

		return greenwhichMeanSiderealTimeDegrees;
	}

	std::string DateTime::ToString() const noexcept {

		char buff[24];
		buff[23] = '\0';
		sprintf_s(buff, sizeof(buff),
			"%02d.%02d.%04d - %02d:%02d:%02d",
			static_cast<int>(Day),
			static_cast<int>(Month),
			static_cast<int>(Year),
			static_cast<int>(Hour),
			static_cast<int>(Minute),
			static_cast<int>(Second));

		return std::string{ buff };
	}
}