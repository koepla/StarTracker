#ifndef STARTRACKER_TRACKABLEBODYVIEW_H
#define STARTRACKER_TRACKABLEBODYVIEW_H

#include <StarEngine/StarEngine.hpp>

#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/AssetDataBase.hpp>
#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Input.hpp>
#include <StarTracker/Core/Tracker.hpp>
#include <StarTracker/Core/BodyLibrary.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Utils/Serial/Package.hpp>
#include <StarTracker/Utils/Serial/Serial.hpp>

#include <StarTracker/Utils/GeoLocation.hpp>
#include <StarTracker/Utils/HttpRequest.hpp>
#include <StarTracker/Utils/Stopwatch.hpp>
#include <StarTracker/Utils/File.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <thread>

namespace StarTracker {

    class TrackableBodyView : public Core::View {

    private:
        double trackingDuration;
        Core::Tracker tracker;
        Utils::LocationService::Location observer;
        std::shared_ptr<Core::BodyLibrary> bodyLibrary;

    public:
        explicit TrackableBodyView(void* nativeWindowHandle) noexcept;

        virtual void OnInit() noexcept override;
        virtual void OnUpdate(float deltaTime) noexcept override;
        virtual void OnDestroy() noexcept override;

    private:
        void reconnectToTracker() noexcept;

        void drawTrackerInfoCard(const glm::vec2& size) noexcept;
        void drawTrackingDurationCard(const glm::vec2& size) noexcept;
        void drawTrackingMenu(Core::BodyLibraryEntry entry, std::string_view title) noexcept;
        void drawAddEntryMenu(std::string_view title) noexcept;

        [[nodiscard]] bool drawCelestialBodyCard(Core::BodyLibraryEntry entry, const glm::vec2& size) noexcept;
    };
}

#endif // STARTRACKER_TRACKABLEBODYVIEW_H
