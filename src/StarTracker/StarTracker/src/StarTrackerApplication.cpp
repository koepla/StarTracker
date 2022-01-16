#include "StarTrackerApplication.hpp"

namespace StarTracker {

    StarTrackerApplication::StarTrackerApplication(const Core::ApplicationData& applicationData) noexcept : Core::Application{ applicationData }, trackableBodyView{ nullptr } {
    
        trackableBodyView.SetNativeWindowHandle(GetWindow().GetNativeHandle());
        AddToViewList(&trackableBodyView);
    }
}