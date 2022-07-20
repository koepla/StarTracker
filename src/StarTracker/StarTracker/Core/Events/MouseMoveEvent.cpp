#include "MouseMoveEvent.hpp"

namespace StarTracker::Core::Events {

    MouseMoveEvent::MouseMoveEvent(double x, double y) noexcept : x{ x }, y{ y } {

    }

    double MouseMoveEvent::GetX() const noexcept {

        return x;
    }

    double MouseMoveEvent::GetY() const noexcept {

        return y;
    }

    std::string_view MouseMoveEvent::GetName() const noexcept {

        return std::string_view{ "MouseMoveEvent" };
    }
}