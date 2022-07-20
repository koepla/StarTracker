#include "MouseScrollEvent.hpp"

namespace StarTracker::Core::Events {

    MouseScrollEvent::MouseScrollEvent(double dx, double dy) noexcept : dx{ dx }, dy{ dy } {

    }

    double MouseScrollEvent::GetDeltaX() const noexcept {

        return dx;
    }

    double MouseScrollEvent::GetDeltaY() const noexcept {

        return dy;
    }

    std::string_view MouseScrollEvent::GetName() const noexcept {

        return std::string_view{ "MouseScrollEvent" };
    }
}