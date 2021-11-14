#ifndef STARAPI_EPHEMERIS_SOLARSYSTEMBODY_H
#define STARAPI_EPHEMERIS_SOLARSYSTEMBODY_H

#include "CelestialBody.hpp"
#include "KeplerianElements.hpp"

namespace StarTracker::Ephemeris {

	class SolarSystemBody : public CelestialBody
	{
	private:
		KeplerianElements keplerElements;
		KeplerianElements keplerElementsCentury;

	public:
		SolarSystemBody(const KeplerianElements& keplerElements, const KeplerianElements& keplerElementsCentury) noexcept;
		SolarSystemBody(const std::string& name, const KeplerianElements& keplerElements, const KeplerianElements& keplerElementsCentury) noexcept;
		
		/**
		* Computes the spherical position of the solar system body
		*
		* @param date date and time for the computation
		*
		* @return the computed spherical coordinates
		*
		*/
		[[nodiscard]] virtual Coordinates::Spherical GetSphericalPosition(const DateTime& date) const noexcept override;

	private:
		[[nodiscard]] double computeEccentricAnomaly(double meanAnomaly, double eccentricity, double eps = 1e-12) const noexcept;
		[[nodiscard]] std::pair<double, double> computeTrueAnomalyAndDistance(double semiMajorAxis, double eccentricAnomaly, double eccentricity) const noexcept;
		[[nodiscard]] double computeEcliptic(double julianCenturies) const noexcept;
		[[nodiscard]] Coordinates::Rectangular computeEarthPos(double julianCenturies) const noexcept;
	};
}

#endif // STARAPI_EPHEMERIS_SOLARSYSTEMBODY_H