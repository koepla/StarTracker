#ifndef STARTRACKER_CORE_EVENTS_EVENT_H
#define STARTRACKER_CORE_EVENTS_EVENT_H

#include <string>

namespace StarTracker::Core::Events {

	class Event {

	public:
		[[nodiscard]] virtual std::string_view GetName() const noexcept = 0;
	};
}

#endif // STARTRACKER_CORE_EVENTS_EVENT_H