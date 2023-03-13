#include <cassert>
#include <cstdint>
#include <ctime>
#include <string>

#include "date-time.hpp"
#include "math.hpp"

#include <fmt/format.h>

namespace {

    constexpr s64 DaysInMonth(const DateTime& date) noexcept {
        switch (date.Month) {
            case 1:
                return 31;
            case 2: {
                if (date.Year % 4 == 0) {
                    if (date.Year % 100 == 0) {
                        if (date.Year % 400 == 0) {
                            return 29;
                        }
                        return 28;
                    }
                    return 29;
                }
                return 28;
            }
            case 3:
                return 31;
            case 4:
                return 30;
            case 5:
                return 31;
            case 6:
                return 30;

            case 7:
            case 8:
                return 31;
            case 9:
                return 30;

            case 10:
                return 31;
            case 11:
                return 30;
            case 12:
                return 31;
            default:
                return 0;
        }
        return 0;
    }
}// namespace

using SystemClock = std::chrono::system_clock;

DateTime::DateTime(s64 year, s64 month, s64 day, s64 hour, s64 minute, s64 second, s64 millisecond) noexcept
    : Year{ year },
      Month{ month },
      Day{ day },
      Hour{ hour },
      Minute{ minute },
      Second{ second },
      Millisecond{ millisecond } { }

void DateTime::AddYears(s64 years) noexcept {
    Year += years;
}

void DateTime::AddMonths(s64 months) noexcept {
    const auto potentialYears = months / 12;
    if (math::Abs(static_cast<f64>(potentialYears)) > 0) {
        AddYears(potentialYears);
        AddMonths(months % 12);
        return;
    }

    if (months > 0) {
        if (Month + months > 12) {
            AddYears(1);
            AddMonths(months - 12);
        } else {
            Month += months;
        }
    } else if (months < 0) {
        if (Month + months < 1) {
            AddYears(-1);
            AddMonths(months + 12);
        } else {
            Month += months;
        }
    }
}

void DateTime::AddDays(s64 days) noexcept {
    addDays(days, DaysInMonth(*this));
}

void DateTime::AddHours(s64 hours) noexcept {
    const auto potentialDays = hours / 24;
    if (math::Abs(static_cast<f64>(potentialDays)) > 0) {
        AddDays(potentialDays);
        AddHours(hours % 24);
        return;
    }

    if (hours > 0) {
        if (Hour + hours > 23) {
            AddDays(1);
            AddHours(hours - 24);
        } else {
            Hour += hours;
        }
    } else if (hours < 0) {
        if (Hour + hours < 0) {
            AddDays(-1);
            AddHours(hours + 24);
        } else {
            Hour += hours;
        }
    }
}

void DateTime::AddMinutes(s64 minutes) noexcept {
    const auto potentialHours = minutes / 60;
    if (math::Abs(static_cast<f64>(potentialHours)) > 0) {
        AddHours(potentialHours);
        AddMinutes(minutes % 60);
        return;
    }

    if (minutes > 0) {
        if (Minute + minutes > 59) {
            AddHours(1);
            AddMinutes(minutes - 60);
        } else {
            Minute += minutes;
        }
    } else if (minutes < 0) {
        if (Minute + minutes < 0) {
            AddHours(-1);
            AddMinutes(minutes + 60);
        } else {
            Minute += minutes;
        }
    }
}

void DateTime::AddSeconds(s64 seconds) noexcept {
    const auto potentialMinutes = seconds / 60;
    if (math::Abs(static_cast<f64>(potentialMinutes)) > 0) {
        AddMinutes(potentialMinutes);
        AddSeconds(seconds % 60);
        return;
    }

    if (seconds > 0) {
        if (Second + seconds > 59) {
            AddMinutes(1);
            AddSeconds(seconds - 60);
        } else {
            Second += seconds;
        }
    } else if (seconds < 0) {
        if (Second + seconds < 0) {
            AddMinutes(-1);
            AddSeconds(seconds + 60);
        } else {
            Second += seconds;
        }
    }
}

void DateTime::Add(s64 number, Unit unit) noexcept {
    switch (unit) {
        case Unit::Seconds:
            AddSeconds(number);
            break;
        case Unit::Minutes:
            AddMinutes(number);
            break;
        case Unit::Hours:
            AddHours(number);
            break;
        case Unit::Days:
            AddDays(number);
            break;
        case Unit::Months:
            AddMonths(number);
            break;
        case Unit::Years:
            AddYears(number);
            break;
    }
}

bool DateTime::IsValid() const noexcept {
    if (Year < 0 || Year > 9999) {
        return false;
    }
    if (Month < 0 || Month > 12) {
        return false;
    }
    if (Day < 1 || Day > DaysInMonth(*this)) {
        return false;
    }
    if (Hour < 0 || Hour > 23) {
        return false;
    }
    if (Minute < 0 || Minute > 59) {
        return false;
    }
    if (Second < 0 || Second > 59) {
        return false;
    }
    if (Millisecond < 0) {
        return false;
    }
    return true;
}

void DateTime::addDays(s64 days, s64 monthDays) noexcept {
    if (days > 0) {
        if (Day + days > monthDays) {
            AddMonths(1);
            AddDays(days - monthDays);
        } else {
            Day += days;
        }
    } else if (days < 0) {
        if (Day + days < 1) {
            AddMonths(-1);
            AddDays(days + DaysInMonth(*this));
        } else {
            Day += days;
        }
    }
}

DateTime DateTime::Now() noexcept {
    const auto now = SystemClock::now();
    const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds);
    const auto time = SystemClock::to_time_t(now);
    const auto ltm = localtime(&time);
    return DateTime{ static_cast<s64>(ltm->tm_year) + 1900,
                     static_cast<s64>(ltm->tm_mon) + 1,
                     static_cast<s64>(ltm->tm_mday),
                     static_cast<s64>(ltm->tm_hour),
                     static_cast<s64>(ltm->tm_min),
                     static_cast<s64>(ltm->tm_sec),
                     milliseconds.count() };
}

DateTime DateTime::Utc() noexcept {
    const auto now = SystemClock::now();
    const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    const auto fraction = now - seconds;
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
    const auto time = SystemClock::to_time_t(now);
    const auto ltm = gmtime(&time);
    return DateTime{ static_cast<s64>(ltm->tm_year) + 1900,
                     static_cast<s64>(ltm->tm_mon) + 1,
                     static_cast<s64>(ltm->tm_mday),
                     static_cast<s64>(ltm->tm_hour),
                     static_cast<s64>(ltm->tm_min),
                     static_cast<s64>(ltm->tm_sec),
                     millis.count() };
}

DateTime DateTime::Utc(DateTime localTime) noexcept {
    const auto difference = Difference(Now(), Utc());
    localTime.AddSeconds(difference);
    return localTime;
}

s64 DateTime::Difference(const DateTime& a, const DateTime& b) noexcept {
    return b.Unix() - a.Unix();
}

f64 DateTime::JulianDayNumber(DateTime date) noexcept {
    return MeanJulianDayNumber(date) + 2400000.5;
}

f64 DateTime::MeanJulianDayNumber(DateTime date) noexcept {
    s64 b;
    if (date.Month <= 2) {
        date.Month += 12;
        --date.Year;
    }
    if (10000 * date.Year + 100 * date.Month + date.Day <= 15821004) {
        // julian calender
        b = -2 + ((date.Year + 4716) / 4) - 1179;
    } else {
        // gregorian calender
        b = date.Year / 400 - date.Year / 100 + date.Year / 4;
    }

    const auto realHour = static_cast<f64>(date.Hour) + static_cast<f64>(date.Minute) / 60.0 +
                          static_cast<f64>(date.Second) / 3.6e3 + static_cast<f64>(date.Millisecond) / 3.6e6;
    return 365.0 * static_cast<f64>(date.Year) - 679004.0 + static_cast<f64>(b) +
           std::floor(30.6001 * static_cast<f64>(date.Month + 1)) + static_cast<f64>(date.Day) +
           static_cast<f64>(realHour) / 24.0;
}

f64 DateTime::JulianCenturies(const DateTime& date, bool floor) noexcept {
    const auto jdn = floor ? std::floor(JulianDayNumber(date)) : JulianDayNumber(date);
    return (jdn - 2451545.0) / 36525.0;
}

f64 DateTime::BesselEpoch(const DateTime& date) noexcept {
    return 1900.0 + (JulianDayNumber(date) - 2415020.31352) / 365.242198781;
}

f64 DateTime::GreenwichMeanSiderealTime(const DateTime& utc) noexcept {
    constexpr auto secondsInDay = 86400.0;
    const auto meanJulianDayNumber = MeanJulianDayNumber(utc);
    const auto meanJulianDayNumberFloor = floor(meanJulianDayNumber);
    const auto UT = secondsInDay * (meanJulianDayNumber - meanJulianDayNumberFloor);
    const auto T = (meanJulianDayNumber - 51544.5) / 36525.0;
    const auto T_0 = (meanJulianDayNumberFloor - 51544.5) / 36525.0;

    const auto greenwichMeanSiderealTime =
            24110.54841 + 8640184.812866 * T_0 + 1.0027379093 * UT + (0.093104 - 6.2e-6 * T) * T * T;
    const auto greenwichMeanSiderealTimeDegrees =
            math::Degrees((math::PI2 / secondsInDay) * math::Mod(greenwichMeanSiderealTime, secondsInDay));
    return math::Mod(greenwichMeanSiderealTimeDegrees, 360.0);
}

std::time_t DateTime::Unix() const noexcept {
    std::time_t rawTime = 0;
    tm* timeInfo = nullptr;
    std::time(&rawTime);
    timeInfo = localtime(&rawTime);
    timeInfo->tm_year = static_cast<int>(Year - 1900);
    timeInfo->tm_mon = static_cast<int>(Month - 1);
    timeInfo->tm_mday = static_cast<int>(Day);
    timeInfo->tm_hour = static_cast<int>(Hour);
    timeInfo->tm_min = static_cast<int>(Minute);
    timeInfo->tm_sec = static_cast<int>(Second);
    return mktime(timeInfo);
}


std::string DateTime::ToString() const noexcept {
    return fmt::format("{:02d}.{:02d}.{:04d} - {:02d}:{:02d}:{:02d}", static_cast<int>(Day), static_cast<int>(Month),
                       static_cast<int>(Year), static_cast<int>(Hour), static_cast<int>(Minute),
                       static_cast<int>(Second));
}

std::string DateTimeUnitToString(DateTime::Unit unit) noexcept {
    switch (unit) {
        case DateTime::Unit::Seconds:
            return "Seconds";
        case DateTime::Unit::Minutes:
            return "Minutes";
        case DateTime::Unit::Hours:
            return "Hours";
        case DateTime::Unit::Days:
            return "Days";
        case DateTime::Unit::Months:
            return "Months";
        case DateTime::Unit::Years:
            return "Years";
    }
    return "";
}


bool operator==(const DateTime& left, const DateTime& right) noexcept {
    return left.Second == right.Second && left.Minute == right.Minute && left.Hour == right.Hour &&
           left.Day == right.Day && left.Month == right.Month && left.Year == right.Year;
}

bool operator!=(const DateTime& left, const DateTime& right) noexcept {
    return !(left == right);
}

bool operator<(const DateTime& left, const DateTime& right) noexcept {
    if (left.Year < right.Year) {
        return true;
    }
    if (left.Year > right.Year) {
        return false;
    }
    if (left.Month < right.Month) {
        return true;
    }
    if (left.Month > right.Month) {
        return false;
    }
    if (left.Day < right.Day) {
        return true;
    }
    if (left.Day > right.Day) {
        return false;
    }
    if (left.Hour < right.Hour) {
        return true;
    }
    if (left.Hour > right.Hour) {
        return false;
    }
    if (left.Minute < right.Minute) {
        return true;
    }
    if (left.Minute > right.Minute) {
        return false;
    }
    if (left.Second < right.Second) {
        return true;
    }
    if (left.Second > right.Second) {
        return false;
    }
    return false;
}

bool operator<=(const DateTime& left, const DateTime& right) noexcept {
    return left < right || left == right;
}

bool operator>(const DateTime& left, const DateTime& right) noexcept {
    return !(left <= right);
}

bool operator>=(const DateTime& left, const DateTime& right) noexcept {
    return left > right || left == right;
}
