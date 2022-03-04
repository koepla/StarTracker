#include "StarTrackerApplication.hpp"

namespace StarTracker {

    StarTrackerApplication::StarTrackerApplication(const Core::ApplicationData& applicationData) noexcept : Core::Application{ applicationData }, trackableBodyView{ nullptr }, experimentalView{ nullptr } {
    
        experimentalView.SetNativeWindowHandle(GetWindow().GetNativeHandle());
        trackableBodyView.SetNativeWindowHandle(GetWindow().GetNativeHandle());
        RegisterView(&experimentalView);
        RegisterView(&trackableBodyView);
    }
}