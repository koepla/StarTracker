#include "EventDispatcher.hpp"

namespace StarTracker::Core::Events {

	EventDispatcher::EventDispatcher() noexcept : eventHandlerList{} {

	}

	void EventDispatcher::DispatchEvent(const Event& event) noexcept {

		for (auto& handler : eventHandlerList) {

			handler(event);
		}
	}

	void EventDispatcher::RegisterEventHandler(const EventHandler& eventHandler) noexcept {

		eventHandlerList.emplace_back(eventHandler);
	}
}
