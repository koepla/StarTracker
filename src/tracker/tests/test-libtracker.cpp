#include <fmt/format.h>
#include <gtest/gtest.h>
#include <libtracker/libtracker.hpp>

TEST(Tracker, Package) {
    // Test libtracker Utils::Package class and functions
    {
        constexpr ephemeris::Horizontal position{ 17.38, 154.92 };
        Pack128 package{ Command::Move };
        package.Push(position);

        const auto readResult = package.Read<ephemeris::Horizontal>(0);
        ASSERT_EQ(package.GetSize(), sizeof readResult);
        ASSERT_DOUBLE_EQ(readResult.Azimuth, position.Azimuth);
        ASSERT_DOUBLE_EQ(readResult.Altitude, position.Altitude);
    }
    {
        constexpr ephemeris::Horizontal moon{ 17.38, 154.92 };
        constexpr ephemeris::Horizontal jupiter{ 34.71, 228.59 };
        Pack128 package{ Command::Move };
        package.Push(moon);
        package.Push(jupiter);

        const auto moonResult = package.Read<ephemeris::Horizontal>(0);
        const auto jupiterResult = package.Read<ephemeris::Horizontal>(1);
        ASSERT_EQ(package.GetSize(), sizeof moonResult + sizeof jupiterResult);
        ASSERT_DOUBLE_EQ(moonResult.Azimuth, moon.Azimuth);
        ASSERT_DOUBLE_EQ(moonResult.Altitude, moon.Altitude);
        ASSERT_DOUBLE_EQ(jupiterResult.Azimuth, jupiter.Azimuth);
        ASSERT_DOUBLE_EQ(jupiterResult.Altitude, jupiter.Altitude);
    }
    {
        constexpr std::array<ephemeris::Horizontal, 3> targets = { ephemeris::Horizontal{ 19.22, 344.71 },
                                                                   ephemeris::Horizontal{ 54.67, 9.48 },
                                                                   ephemeris::Horizontal{ 12.17, 231.38 } };

        Pack128 package{ Command::Move };
        package.PushRange(targets.data(), targets.size());

        std::array<ephemeris::Horizontal, 3> readResult{};
        std::memcpy(readResult.data(), package.ReadRange<ephemeris::Horizontal>(0), sizeof targets);

        ASSERT_EQ(targets.size(), readResult.size());
        for (std::size_t i = 0; i < readResult.size(); i++) {
            const auto& input = targets[i];
            const auto& result = readResult[i];
            ASSERT_DOUBLE_EQ(input.Azimuth, result.Azimuth);
            ASSERT_DOUBLE_EQ(input.Altitude, result.Altitude);
        }
    }
}
