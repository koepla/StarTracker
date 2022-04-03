#include "FixedBody.hpp"

namespace StarTracker::Ephemeris {

	FixedBody::FixedBody(const std::string& name, const std::string& textureHandle, const Coordinates::Spherical& position) noexcept : FixedBody{ name, "", textureHandle, position } {

	}

	FixedBody::FixedBody(const std::string& name, const std::string& designation, const std::string& textureHandle, const Coordinates::Spherical& position) noexcept : CelestialBody{ name, designation, textureHandle }, position{ position } {

	}

	Coordinates::Spherical FixedBody::GetSphericalPosition(const DateTime& date) const noexcept {

		return position;
	}
}