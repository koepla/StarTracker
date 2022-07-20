#ifndef STARTRACKER_STARTRACKERVIEW_H
#define STARTRACKER_STARTRACKERVIEW_H

#include <StarTracker/ModelView.hpp>
#include <StarTracker/ImageProcessingView.hpp>
#include <StarTracker/TrackableBodyView.hpp>
#include <StarTracker/AstronomicalView.hpp>

namespace StarTracker {

    class StarTrackerView : public Core::View {

    private:
        std::shared_ptr<ModelView> modelView;
        std::shared_ptr<ImageProcessingView> imageProcessingView;
        std::shared_ptr<TrackableBodyView> trackableBodyView;
        std::shared_ptr<AstronomicalView> astronomicalView;

    public:
        explicit StarTrackerView(void* nativeWindowHandle) noexcept;

        virtual void OnInit() noexcept override;
        virtual void OnUpdate(float deltaTime) noexcept override;
        virtual void OnDestroy() noexcept override;
    };
}

#endif // STARTRACKER_STARTRACKERVIEW_H