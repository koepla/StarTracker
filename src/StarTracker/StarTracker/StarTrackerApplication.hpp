#ifndef STARTRACKER_STARTRACKERAPPLICATION_H
#define STARTRACKER_STARTRACKERAPPLICATION_H

#include <StarTracker/StarTrackerView.hpp>

namespace StarTracker {

    class StarTrackerApplication : public Core::Application {

    private:
        std::unique_ptr<StarTrackerView> starTrackerView;

    public:
        explicit StarTrackerApplication(const Core::ApplicationData& applicationData) noexcept;
    };
}

#endif // STARTRACKER_STARTRACKERAPPLICATION_H

