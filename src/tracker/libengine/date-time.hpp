#ifndef LIBENGINE_DATETIME_H
#define LIBENGINE_DATETIME_H

#include <chrono>
#include <string>

#include "utility/types.hpp"

/**
 * @brief Date struct for simplification and convenience
 */
class DateTime {
public:
    enum class Unit : std::uint16_t { Seconds, Minutes, Hours, Days, Months, Years };

    s64 Year;
    s64 Month;
    s64 Day;
    s64 Hour;
    s64 Minute;
    s64 Second;
    s64 Millisecond;

    DateTime(s64 year, s64 month, s64 day, s64 hour, s64 minute, s64 second, s64 millisecond = 0) noexcept;

    /**
     * @brief Adds or subtracts the specified number of years to or from the date
     * @param years Number of years
     */
    void AddYears(s64 years) noexcept;

    /**
     * @brief Adds or subtracts the specified number of months to or from the date
     * @param months Number of months
     */
    void AddMonths(s64 months) noexcept;

    /**
     * @brief Adds or subtracts the specified number of days to or from the date
     * @param days Number of days
     */
    void AddDays(s64 days) noexcept;

    /**
     * @brief Adds or subtracts the specified number of hours to or from the date
     * @param hours Number of hours
     */
    void AddHours(s64 hours) noexcept;

    /**
     * @brief Adds or subtracts the specified number of minutes to or from the date
     * @param minutes Number of minutes
     */
    void AddMinutes(s64 minutes) noexcept;

    /**
     * @brief Adds or subtracts the specified number of seconds to or from the date
     * @param seconds Number of seconds
     */
    void AddSeconds(s64 seconds) noexcept;

    /**
     * @brief Adds or subtracts the specified number of units to or from the date
     * @param number Number of units
     * @param unit Unit
     */
    void Add(s64 number, Unit unit) noexcept;

    /**
     * Checks if the current DateTime instance has a valid date and time
     * @return bool
     */
    bool IsValid() const noexcept;

private:
    void addDays(s64 days, s64 monthDays) noexcept;

public:
    /**
     * @brief Returns the local date-time
     * @return Local date-time
     */
    static DateTime Now() noexcept;

    /**
     * @brief Returns the current utc-time
     * @return Utc-time
     */
    static DateTime Utc() noexcept;

    /**
     * @brief Returns the relative utc-time
     * @param localTime The local time to be converted
     * @return Utc-time
     */
    static DateTime Utc(DateTime localTime) noexcept;

    /**
     * @brief Calculates the difference in seconds
     * @param a First date-time
     * @param b Second date-time
     * @return Difference in seconds
     */
    static s64 Difference(const DateTime& a, const DateTime& b) noexcept;

    /**
     * @brief Calculates the julian day number for the given date
     * @param date Date for the calculation
     * @return Julian day number
     */
    static f64 JulianDayNumber(DateTime date) noexcept;

    /**
     * @brief Calculates the mean julian day number for the given date
     * @param date Date for the calculation
     * @return Mean julian day number
     */
    static f64 MeanJulianDayNumber(DateTime date) noexcept;

    /**
     * @brief Calculates the julian centuries via the julian day number
     * @param date Date for the calculation
     * @param floor Basically std::floor is called when this is set to true
     */
    static f64 JulianCenturies(const DateTime& date, bool floor = false) noexcept;

    /**
     * @brief Returns the bessel epoch for the given date
     * @param date Date for the calculation
     * @return Bessel epoch
     */
    static f64 BesselEpoch(const DateTime& date) noexcept;

    /**
     * @brief Calculates the greenwich mean sidereal time in degrees
     * @param utc The relative utc time for the calculation
     * @return Sidereal time in degrees
     */
    static f64 GreenwichMeanSiderealTime(const DateTime& utc) noexcept;

    /**
     * @brief Computes the unix timestamp for the date
     */
    std::time_t Unix() const noexcept;

    /**
     * @brief Formats the date as a string
     * @return Formatted date
     */
    std::string ToString() const noexcept;

    friend bool operator==(const DateTime& left, const DateTime& right) noexcept;
    friend bool operator!=(const DateTime& left, const DateTime& right) noexcept;
    friend bool operator<(const DateTime& left, const DateTime& right) noexcept;
    friend bool operator<=(const DateTime& left, const DateTime& right) noexcept;
    friend bool operator>(const DateTime& left, const DateTime& right) noexcept;
    friend bool operator>=(const DateTime& left, const DateTime& right) noexcept;
};

/**
 * @brief Converts the specified Unit to a string
 * @param unit Enum value
 * @return String representation of the enum value
 */
std::string DateTimeUnitToString(DateTime::Unit unit) noexcept;

#endif// LIBENGINE_DATETIME_H