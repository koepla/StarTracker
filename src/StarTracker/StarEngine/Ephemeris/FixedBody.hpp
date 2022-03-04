#ifndef STARENGINE_EPHEMERIS_FIXEDBODY_H
#define STARENGINE_EPHEMERIS_FIXEDBODY_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>

namespace StarTracker::Ephemeris {

	class FixedBody : public CelestialBody {

	private:
		Coordinates::Spherical position;

	public:
		explicit FixedBody(const Coordinates::Spherical& position) noexcept;
		FixedBody(const std::string& name, const Coordinates::Spherical& position) noexcept;
		FixedBody(const std::string& name, const std::string& designation, const Coordinates::Spherical& position) noexcept;

		/**
		* Computes the spherical position of the celestial body
		*
		* @param date date and time for the computation
		*
		* @return the computed spherical coordinates
		*
		*/
		[[nodiscard]] virtual Coordinates::Spherical GetSphericalPosition(const DateTime& date) const noexcept override;
	};
}

#endif // STARENGINE_EPHEMERIS_FIXEDBODY_H