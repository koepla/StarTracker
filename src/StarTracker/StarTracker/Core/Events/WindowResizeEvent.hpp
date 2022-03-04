#ifndef STARTRACKER_CORE_EVENTS_WINDOWRESIZEEVENT_H
#define STARTRACKER_CORE_EVENTS_WINDOWRESIZEEVENT_H

#include <StarTracker/Core/Events/Event.hpp>

namespace StarTracker::Core::Events {

	class WindowResizeEvent : public Event {

	private:
		std::int32_t width;
		std::int32_t height;

	public:
		WindowResizeEvent(std::int32_t width, std::int32_t height) noexcept;

		[[nodiscard]] std::int32_t GetWidth() const noexcept;
		[[nodiscard]] std::int32_t GetHeight() const noexcept;
		[[nodiscard]] virtual std::string_view GetName() const noexcept override;
	};
}

#endif // STARTRACKER_CORE_EVENTS_WINDOWRESIZEEVENT_H

