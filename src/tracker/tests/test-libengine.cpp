#include <fstream>
#include <fmt/format.h>
#include <gtest/gtest.h>
#include <libengine/libengine.hpp>



TEST(Engine, Math) {
    // Test libengine math functions
    // We don't need to test Sine, Cosine, Tangent and their inverse function,
    // as they call functions from the standard library, and we already test for math::Degrees and math::Radians
    const auto test = [](std::string_view name, double a, double b) {
        const auto trace = fmt::format("{} with input a => {}, b => {}", name, a, b);
        SCOPED_TRACE(trace);
        ASSERT_DOUBLE_EQ(a, b);
    };

    // Abs
    test("math::Abs negative", math::Abs(-69.0), 69.0);
    test("math::Abs positive", math::Abs(420.0), 420.0);

    // Radians and Degrees
    test("math::Radians 180", math::Radians(180.0), math::PI);
    test("math::Radians 360", math::Radians(360.0), math::PI2);
    test("math::Degrees pi", math::Degrees(math::PI / 2.0), 90.0);
    test("math::Degrees 2pi", math::Degrees(math::PI), 180.0);

    // Mod
    test("math::Mod greater", math::Mod(400.0, 360.0), 40.0);
    test("math::Mod less", math::Mod(-20.0, 180.0), 160.0);

    // DaaToDegrees and HmsToDegrees
    test("math::DaaToDegrees arc-minutes", math::DaaToDegrees(18.0, 30.0, 0.0), 18.5);
    test("math::DaaToDegrees arc-seconds hack", math::DaaToDegrees(67.0, 44.0, 60.0), 67.75);
    test("math::HmsToDegrees minutes", math::HmsToDegrees(12.0, 1, 0.0), 180.25);
}

TEST(Engine, Transform) {
    // Test libengine transform functions
    constexpr double sqrt3 = 1.73205080756887729352744634150587236694280525381038062805580697945;
    constexpr ephemeris::Vector3 input{ 1.0, 1.0, 1.0 };
    constexpr ephemeris::Equatorial expected{ // we do not test for declination as its value is not straight forward
                                              sqrt3, 45.0, 0.0
    };
    const auto result = VectorToEquatorial(input);
    ASSERT_DOUBLE_EQ(expected.Radius, result.Radius);
    ASSERT_DOUBLE_EQ(expected.RightAscension, result.RightAscension);
}

TEST(Engine, MatrixMultiplication) {
    ephemeris::Matrix3x3 a{};
    a[0] = { 5, -3, 11 };
    a[1] = { 5, 6, 22 };
    a[2] = { 8, 19, 0 };

    ephemeris::Matrix3x3 b{};
    b[0] = { 12, 3, -4 };
    b[1] = { 5, 16, 9 };
    b[2] = { 8, -3, 4 };

    ephemeris::Matrix3x3 result{};
    result[0] = { 133, -66, -3 };
    result[1] = { 266, 45, 122 };
    result[2] = { 191, 328, 139 };

    ASSERT_EQ(a * b, result);
}

TEST(Engine, MatrixTransposition) {
    ephemeris::Matrix3x3 a{};
    a[0] = { 1, 2, 3 };
    a[1] = { 2, 1, 3 };
    a[2] = { 3, 2, 1 };

    ephemeris::Matrix3x3 b{};
    b[0] = { 1, 2, 3 };
    b[1] = { 2, 1, 2 };
    b[2] = { 3, 3, 1 };

    ASSERT_EQ(a.Transpose(), b);
}

TEST(Engine, Rotation) {
    constexpr ephemeris::Vector3 input{ 0.5, 0.0, 1.0 };
    const auto rotated = ephemeris::RotationMatrix(ephemeris::RotationAxis::Y, 90.0) * input;
    ASSERT_DOUBLE_EQ(rotated.X, 1.0);
    ASSERT_DOUBLE_EQ(rotated.Y, 0.0);
    ASSERT_DOUBLE_EQ(rotated.Z, -0.5);
}

TEST(Engine, DateTimeAddSecondsPositive) {
    DateTime date{ 1990, 12, 31, 23, 59, 59 };
    date.AddSeconds(1);
    const DateTime expected{ 1991, 1, 1, 0, 0, 0 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddSecondsNegative) {
    DateTime date{ 1995, 1, 1, 0, 0, 4 };
    date.AddSeconds(-5);
    const DateTime expected{ 1994, 12, 31, 23, 59, 59 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddMinutesPositive) {
    DateTime date{ 2004, 5, 31, 23, 58, 0 };
    date.AddMinutes(2);
    const DateTime expected{ 2004, 6, 1, 0, 0, 0 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddMinutesNegative) {
    DateTime date{ 2004, 5, 31, 0, 55, 13 };
    date.AddMinutes(-57);
    const DateTime expected{ 2004, 5, 30, 23, 58, 13 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddHoursPositive) {
    DateTime date{ 2020, 1, 31, 12, 0, 13 };
    date.AddHours(37);
    const DateTime expected{ 2020, 2, 2, 1, 0, 13 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddHoursNegative) {
    DateTime date{ 1969, 7, 20, 15, 34, 27 };
    date.AddHours(-13);
    const DateTime expected{ 1969, 7, 20, 2, 34, 27 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddDaysPositive) {
    DateTime date{ 2007, 2, 27, 9, 1, 48 };
    date.AddDays(2);
    const DateTime expected{ 2007, 3, 1, 9, 1, 48 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddDaysNegative) {
    DateTime date{ 1988, 3, 4, 7, 0, 32 };
    date.AddDays(-5);
    const DateTime expected{ 1988, 2, 28, 7, 0, 32 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddDaysPositiveLapYear) {
    {
        DateTime date{ 2004, 2, 28, 0, 0, 0 };
        date.AddDays(2);
        const DateTime expected{ 2004, 3, 1, 0, 0, 0 };
        ASSERT_EQ(date, expected);
    }
    {
        DateTime date{ 2004, 2, 28, 0, 0, 0 };
        date.AddDays(63);
        const DateTime expected{ 2004, 5, 1, 0, 0, 0 };
        ASSERT_EQ(date, expected);
    }
}

TEST(Engine, DateTimeAddMonthsPositive) {
    DateTime date{ 2002, 4, 10, 12, 12, 12 };
    date.AddMonths(27);
    const DateTime expected{ 2004, 7, 10, 12, 12, 12 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddMonthsNegative) {
    DateTime date{ 2002, 1, 10, 12, 12, 12 };
    date.AddMonths(-25);
    const DateTime expected{ 1999, 12, 10, 12, 12, 12 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddYearsPositive) {
    DateTime date{ 1900, 4, 11, 7, 50, 3 };
    date.AddYears(69);
    const DateTime expected{ 1969, 4, 11, 7, 50, 3 };
    ASSERT_EQ(date, expected);
}

TEST(Engine, DateTimeAddYearsNegative) {
    DateTime date{ 2013, 1, 31, 1, 47, 33 };
    date.AddYears(-4);
    const DateTime expected{ 2009, 1, 31, 1, 47, 33 };
    ASSERT_EQ(date, expected);
}

namespace {

    std::string ReadFile(const std::filesystem::path& path) {
        std::ifstream file(path);
        return std::string{ std::istreambuf_iterator(file), {} };
    }
}// namespace

TEST(Engine, CatalogFindPlanetByName) {
    const auto planetData = ReadFile("assets/ephemeris/planets.json");
    ephemeris::Catalog catalog;
    catalog.ImportPlanets(planetData);
    ASSERT_TRUE(catalog.FindPlanetByName("Mercury") != nullptr);
    ASSERT_TRUE(catalog.FindPlanetByName("Venus") != nullptr);
}

TEST(Engine, CatalogFindFixedByName) {
    const auto ngcData = ReadFile("assets/ephemeris/ngc2000.dat");
    const auto nameData = ReadFile("assets/ephemeris/names.dat");
    ephemeris::Catalog catalog;
    catalog.ImportFixed(ngcData, nameData);
    ASSERT_TRUE(catalog.FindFixedByName("Antennae") != nullptr);
}