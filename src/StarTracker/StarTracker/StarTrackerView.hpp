#ifndef STARTRACKER_STARTRACKERVIEW_H
#define STARTRACKER_STARTRACKERVIEW_H

#include <StarTracker/ExperimentalView.hpp>
#include <StarTracker/TrackableBodyView.hpp>

namespace StarTracker {

    class StarTrackerView : public Core::View {

    private:
        std::unique_ptr<ExperimentalView> experimentalView;
        std::unique_ptr<TrackableBodyView> trackableBodyView;

    public:
        explicit StarTrackerView(void* nativeWindowHandle) noexcept;

        virtual void OnInit() noexcept override;
        virtual void OnUpdate(float deltaTime) noexcept override;
        virtual void OnDestroy() noexcept override;
    };
}

#endif // STARTRACKER_STARTRACKERVIEW_H