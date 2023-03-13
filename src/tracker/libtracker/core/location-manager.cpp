#include "location-manager.hpp"
#include "settings.hpp"

ephemeris::Geographic LocationManager::GetGeographic() noexcept {
    if (!Settings::Contains("Location-Latitude") || !Settings::Contains("Location-Longitude")) {
        return {};
    }
    return { Settings::Get<f64>("Location-Latitude"), Settings::Get<f64>("Location-Longitude") };
}

bool LocationManager::FetchOnline() noexcept {
    try {
        const auto location = GeoLocation::Get();
        Settings::Add("Location-Latitude", location->Latitude);
        Settings::Add("Location-Longitude", location->Longitude);
        Settings::Add("Location-City", location->City);
        Settings::Add("Location-RegionName", location->RegionName);
        Settings::Add("Location-Country", location->Country);
        return true;
    } catch (...) {
        return false;
    }
}

bool LocationManager::IsConfigured() noexcept {
    return Settings::Contains("Location-Latitude") && Settings::Contains("Location-Longitude");
}

bool LocationManager::IsDescriptive() noexcept {
    return Settings::Contains("Location-City") && Settings::Contains("Location-RegionName") &&
           Settings::Contains("Location-Country");
}

std::string LocationManager::GetFormatted() noexcept {
    if (!IsConfigured()) {
        return "Missing information";
    }
    if (IsDescriptive()) {
        return fmt::format("Location: {}, {}, {}", Settings::Get<std::string>("Location-City"),
                           Settings::Get<std::string>("Location-RegionName"),
                           Settings::Get<std::string>("Location-Country"));
    }
    return fmt::format("Location: {} deg, {} deg", Settings::Get<f64>("Location-Latitude"),
                       Settings::Get<f64>("Location-Longitude"));
}
