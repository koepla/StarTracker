#ifndef STARTRACKER_UTILS_GEOLOCATION_H
#define STARTRACKER_UTILS_GEOLOCATION_H

#include <StarTracker/Utils/HttpRequest.hpp>

#include <nlohmann/json.hpp>

#include <string>

namespace StarTracker::Utils::LocationService {

    struct Location {

        std::string Country;
        std::string RegionName;
        std::string City;
        double Latitude;
        double Longitude;
    };

    class GeoLocationException : public std::exception {

    private:
        std::string_view message;

    public:
        explicit GeoLocationException(std::string_view message) noexcept;

        [[nodiscard]] virtual const char* what() const noexcept override;
    };

    class GeoLocation {

    public:
        /**
        * @brief Retrieves the GeoLocation via IP address
        *
        * @see Location
        *
        * @return Location struct
        *
        * @throws GeoLocationException if the HttpRequest fails
        *
        */
        [[nodiscard]] static Location Get() noexcept(false);
    };
}

#endif // STARTRACKER_UTILS_GEOLOCATION_H