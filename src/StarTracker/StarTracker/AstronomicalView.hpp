#ifndef STARTRACKER_ASTRONOMICALVIEW_H
#define STARTRACKER_ASTRONOMICALVIEW_H

#include <StarEngine/StarEngine.hpp>
#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/AssetDataBase.hpp>
#include <StarTracker/Core/BodyLibrary.hpp>
#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Input.hpp>
#include <StarTracker/Core/View.hpp>
#include <StarTracker/Utils/GeoLocation.hpp>

namespace StarTracker {

    class AstronomicalView : public Core::View {

    private:
        Utils::LocationService::Location observer;
        std::shared_ptr<Core::BodyLibrary> bodyLibrary;

    public:
        explicit AstronomicalView(void* nativeWindowHandle) noexcept;

        virtual void OnInit() noexcept override;
        virtual void OnUpdate(float deltaTime) noexcept override;
        virtual void OnDestroy() noexcept override;

    private:
        void drawPositions() noexcept;
        void drawEphemeris() noexcept;
        void drawGraph() noexcept;;
    };
}

#endif // STARTRACKER_ASTRONOMICALVIEW_H