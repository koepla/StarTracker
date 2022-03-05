#include "StarTrackerApplication.hpp"

namespace StarTracker {

    StarTrackerApplication::StarTrackerApplication(const Core::ApplicationData& applicationData) noexcept : Core::Application{ applicationData }, starTrackerView{ nullptr } {
    
        starTrackerView = std::make_unique<StarTrackerView>(GetWindow().GetNativeHandle());
        RegisterView(starTrackerView.get());
    }
}