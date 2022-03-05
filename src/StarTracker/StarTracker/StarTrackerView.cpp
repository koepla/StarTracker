#include "StarTrackerView.hpp"

namespace StarTracker {

    StarTrackerView::StarTrackerView(void *nativeWindowHandle) noexcept : Core::View{ nativeWindowHandle }, experimentalView{ nullptr }, trackableBodyView{ nullptr } {

        experimentalView = std::make_unique<ExperimentalView>(nativeWindowHandle);
        trackableBodyView = std::make_unique<TrackableBodyView>(nativeWindowHandle);

        const auto application = Core::Application::GetInstance();
        application->RegisterView(experimentalView.get());
        application->RegisterView(trackableBodyView.get());
    }

    void StarTrackerView::OnInit() noexcept {

    }

    void StarTrackerView::OnUpdate(float deltaTime) noexcept {

    }

    void StarTrackerView::OnDestroy() noexcept {

    }
}