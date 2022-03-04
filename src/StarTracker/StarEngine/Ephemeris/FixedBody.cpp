#include "FixedBody.hpp"

namespace StarTracker::Ephemeris {

	FixedBody::FixedBody(const Coordinates::Spherical& position) noexcept : FixedBody{ "Unnamed celestial body", position } {

	}

	FixedBody::FixedBody(const std::string& name, const Coordinates::Spherical& position) noexcept : FixedBody{ name, "None", position } {

	}

	FixedBody::FixedBody(const std::string& name, const std::string& designation, const Coordinates::Spherical& position) noexcept : CelestialBody{ name, designation }, position{ position } {

	}

	Coordinates::Spherical FixedBody::GetSphericalPosition(const DateTime& date) const noexcept {

		return position;
	}
}