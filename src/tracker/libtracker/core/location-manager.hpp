#ifndef LIBTRACKER_CORE_LOCATIONMANAGER_H
#define LIBTRACKER_CORE_LOCATIONMANAGER_H

#include <libengine/ephemeris/coordinates.hpp>

#include "geo-location.hpp"
#include "utility/types.hpp"

class LocationManager {
public:
    /**
     * @brief Returns the geographic location of the user
     * @return Geographic location
     */
    static ephemeris::Geographic GetGeographic() noexcept;

    /**
     * @brief Tries to fetch the location of the user from the internet
     * @return true If the location could be fetched
     * @return false If the location could not be fetched
     */
    static bool FetchOnline() noexcept;

    /**
     * @brief Checks if the settings.json file contains an entry for the location
     * @return true If the necessary settings entries are present
     * @return false If not
     */
    static bool IsConfigured() noexcept;

    /**
     * @brief Checks if the settings.json file contains additional information about the geographic location. This
     * contains the name of the city, the region and the country.
     * @return true If this information is present
     * @return false If not
     */
    static bool IsDescriptive() noexcept;

    /**
     * @brief Returns the location in string representation
     * @return Location string
     */
    static std::string GetFormatted() noexcept;
};

#endif// LIBTRACKER_CORE_LOCATIONMANAGER_H