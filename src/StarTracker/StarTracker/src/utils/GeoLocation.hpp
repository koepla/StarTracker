#ifndef STARTRACKER_UTILS_GEOLOCATION_H
#define STARTRACKER_UTILS_GEOLOCATION_H

#include <string>

namespace Utils::LocationService {

	struct Location {

		std::string Country;
		std::string RegionName;
		std::string City;
		double Latitude;
		double Longitude;
	};

	class GeoLocationException : public std::exception {

	private:
		std::string message;

	public:
		GeoLocationException(std::string&& message);

		[[nodiscard]] virtual const char* what() const noexcept override;
	};

	class GeoLocation {

	public:
		/*
			note:
			This method has a certain degree of inaccuracy, 
			as the location is determined via the IP address
			
			input:
			none

			routine:
			Sends HttpRequest to "https://ip-api.com/json/", parses json to Location struct

			returns:
			Location struct

			throws:
			GeoLocationException

			example: (surrounded by try-catch block) 
			auto location = Utils::LocationService::GeoLocation::Get();
		*/
		[[nodiscard]] static Location Get() noexcept(false);
	};
}

#endif // STARTRACKER_UTILS_GEOLOCATION_H