#include "geo-location.hpp"
#include "arch/http-request.hpp"

GeoLocationException::GeoLocationException(std::string_view message) noexcept : message{ message } { }

const char* GeoLocationException::what() const noexcept {
    return message.data();
}

std::unique_ptr<GeoLocation> GeoLocation::Get() noexcept(false) {
    std::string responseData;

    try {
        // Send request to ""https://ip-api.com/json/"
        responseData = arch::HttpRequest::Get("ip-api.com", "json");
    } catch (const arch::HttpRequestException& e) {
        // If the request fails, throw Exception
        throw GeoLocationException{ fmt::format("Couldn't get location data [Utils::Http::HttpRequest failed => {}]",
                                                e.what()) };
    }

    // Try to parse the responseData to a json object using nlohmann::json
    nlohmann::json jObject = nlohmann::json::parse(responseData, nullptr, false, true);

    // Check if the json object contains the correct key-value pairs
    if (jObject.contains("country") && jObject.contains("regionName") && jObject.contains("city") &&
        jObject.contains("lat") && jObject.contains("lon")) {

        return std::make_unique<GeoLocation>(GeoLocation{
                jObject["country"].get<std::string>(), jObject["regionName"].get<std::string>(),
                jObject["city"].get<std::string>(), jObject["lat"].get<f64>(), jObject["lon"].get<f64>() });
    }
    throw GeoLocationException{ "Couldn't get location data [nlohmann::json parsing failed]" };
}
