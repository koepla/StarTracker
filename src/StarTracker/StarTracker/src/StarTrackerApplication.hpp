#ifndef STARTRACKER_STARTRACKERAPPLICATION_H
#define STARTRACKER_STARTRACKERAPPLICATION_H

#include "ExperimentalView.hpp"
#include "TrackableBodyView.hpp"

namespace StarTracker {

	class StarTrackerApplication : public Core::Application {

	private:
		TrackableBodyView trackableBodyView;
		ExperimentalView experimentalView;

	public:
		explicit StarTrackerApplication(const Core::ApplicationData& applicationData) noexcept;
	};
}


#endif // STARTRACKER_STARTRACKERAPPLICATION_H

