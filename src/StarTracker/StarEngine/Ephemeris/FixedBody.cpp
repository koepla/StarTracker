#include "FixedBody.hpp"

namespace StarTracker::Ephemeris {

    FixedBody::FixedBody(std::string_view name, std::string_view textureHandle, const Coordinates::Spherical& position) noexcept : FixedBody{ name, "", textureHandle, position } {

    }

    FixedBody::FixedBody(std::string_view name, std::string_view designation, std::string_view textureHandle, const Coordinates::Spherical& position) noexcept : CelestialBody{ name, designation, textureHandle }, position{ position } {

    }

    Coordinates::Spherical FixedBody::GetSphericalPosition(const DateTime& date) const noexcept {

        return position;
    }

    std::string FixedBody::GetSerializable() const noexcept {

        nlohmann::json jObject{};
        jObject["Name"] = name;
        jObject["Designation"] = designation;
        jObject["Type"] = "FB";
        jObject["TextureHandle"] = textureHandle;
        jObject["RightAscension"] = position.RightAscension;
        jObject["Declination"] = position.Declination;
        jObject["Radius"] = position.Radius;

        return jObject.dump();
    }
}