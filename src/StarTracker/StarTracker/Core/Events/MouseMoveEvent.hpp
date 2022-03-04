#ifndef STARTRACKER_CORE_EVENTS_MOUSEMOVEEVENT_H
#define STARTRACKER_CORE_EVENTS_MOUSEMOVEEVENT_H

#include <StarTracker/Core/Events/Event.hpp>

namespace StarTracker::Core::Events {

	class MouseMoveEvent : public Event {

	private:
		double x;
		double y;

	public:
		MouseMoveEvent(double x, double y) noexcept;

		[[nodiscard]] double GetX() const noexcept;
		[[nodiscard]] double GetY() const noexcept;
		[[nodiscard]] virtual std::string_view GetName() const noexcept;
	};
}

#endif // STARTRACKER_CORE_EVENTS_MOUSEMOVEEVENT_H

