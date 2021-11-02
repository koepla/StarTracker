#include "GeoLocation.hpp"
#include "HttpRequest.hpp"

namespace StarTracker::Utils::LocationService {

	GeoLocationException::GeoLocationException(std::string&& message) noexcept : message(std::move(message)) {

	}

	const char* GeoLocationException::what() const noexcept {

		return message.c_str();
	}

	Location GeoLocation::Get() noexcept(false) {

		std::string responseData = "";

		try {

			// Send request to ""https://ip-api.com/json/"
			responseData = Http::HttpRequest::Get("ip-api.com", "json");
		}
		catch (const Http::HttpRequestException&) {

			// If the request fails, throw Exception
			throw GeoLocationException("Couldn't get location data [Utils::Http::HttpRequest failed]");
		}

		// Try to parse the responseData to a json object using nlohmann::json
		nlohmann::json jObject = nlohmann::json::parse(responseData);

		// Check if the json object contains the correct key-value pairs
		if (jObject.contains("country") && jObject.contains("regionName") && jObject.contains("city") && jObject.contains("lat") && jObject.contains("lon")) {

			return Location({ jObject["country"].get<std::string>(), jObject["regionName"].get<std::string>(), jObject["city"].get<std::string>(), jObject["lat"].get<double>(), jObject["lon"].get<double>() });
		}
		else {

			throw GeoLocationException("Couldn't get location data [nlohmann::json parsing failed]");
		}
	}
}