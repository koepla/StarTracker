#ifndef LIBTRACKER_UTILS_GEOLOCATION_H
#define LIBTRACKER_UTILS_GEOLOCATION_H

#include <string>

#include <nlohmann/json.hpp>

#include "utility/types.hpp"

class GeoLocationException : public std::exception {
public:
    explicit GeoLocationException(std::string_view message) noexcept;

    const char* what() const noexcept override;

private:
    std::string message;
};

struct GeoLocation {
    std::string Country;
    std::string RegionName;
    std::string City;
    f64 Latitude;
    f64 Longitude;
    /**
     * @brief Retrieves the GeoLocation via IP address
     * @return GeoLocation information
     * @throws GeoLocationException if the HttpRequest fails
     */
    static std::unique_ptr<GeoLocation> Get() noexcept(false);
};

#endif// LIBTRACKER_UTILS_GEOLOCATION_H