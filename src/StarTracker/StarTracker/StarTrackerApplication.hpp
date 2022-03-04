#ifndef STARTRACKER_STARTRACKERAPPLICATION_H
#define STARTRACKER_STARTRACKERAPPLICATION_H

#include <StarTracker/ExperimentalView.hpp>
#include <StarTracker/TrackableBodyView.hpp>

namespace StarTracker {

	class StarTrackerApplication final : public Core::Application {

	private:
		TrackableBodyView trackableBodyView;
		ExperimentalView experimentalView;

	public:
		explicit StarTrackerApplication(const Core::ApplicationData& applicationData) noexcept;
	};
}


#endif // STARTRACKER_STARTRACKERAPPLICATION_H

