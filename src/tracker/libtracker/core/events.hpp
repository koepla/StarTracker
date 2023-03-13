#ifndef LIBTRACKER_CORE_EVENTS_H
#define LIBTRACKER_CORE_EVENTS_H

#include <functional>
#include <string_view>
#include <vector>

#include "input.hpp"
#include "utility/types.hpp"

class Event {
public:
    Event() noexcept = default;
    virtual ~Event() noexcept;
    virtual std::string_view GetName() const noexcept = 0;
};

using EventHandler = std::function<void(const Event&)>;

class EventDispatcher {
public:
    EventDispatcher() noexcept = default;

    void DispatchEvent(const Event& event) const noexcept;
    void RegisterEventHandler(const EventHandler& eventHandler) noexcept;

private:
    std::vector<EventHandler> eventHandlerList;
};

enum class KeyStatus : u16 { None = 0, Pressed, Repeated, Released };

class KeyEvent : public Event {
public:
    KeyEvent(KeyCode keyCode, KeyStatus keyStatus) noexcept;
    ~KeyEvent() noexcept override = default;

    KeyCode GetKeyCode() const noexcept;
    bool IsPressed() const noexcept;
    bool IsRepeated() const noexcept;
    bool IsReleased() const noexcept;
    std::string_view GetName() const noexcept override;

private:
    KeyCode keyCode;
    KeyStatus keyStatus;
};

enum class MouseStatus : u16 { None = 0, Pressed, Released };

class MouseClickEvent : public Event {
public:
    MouseClickEvent(MouseCode mouseCode, MouseStatus mouseStatus) noexcept;
    ~MouseClickEvent() noexcept override = default;

    MouseCode GetMouseCode() const noexcept;
    bool IsPressed() const noexcept;
    bool IsReleased() const noexcept;
    std::string_view GetName() const noexcept override;

private:
    MouseCode mouseCode;
    MouseStatus mouseStatus;
};

class MouseMoveEvent : public Event {
public:
    MouseMoveEvent(f64 x, f64 y) noexcept;
    ~MouseMoveEvent() noexcept override = default;

    f64 GetX() const noexcept;
    f64 GetY() const noexcept;
    std::string_view GetName() const noexcept override;

private:
    f64 x;
    f64 y;
};

class MouseScrollEvent : public Event {
public:
    MouseScrollEvent(f64 dx, f64 dy) noexcept;
    ~MouseScrollEvent() noexcept override = default;

    f64 GetDeltaX() const noexcept;
    f64 GetDeltaY() const noexcept;

    std::string_view GetName() const noexcept override;

private:
    f64 dx;
    f64 dy;
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(s32 width, s32 height) noexcept;
    ~WindowResizeEvent() noexcept override = default;

    s32 GetWidth() const noexcept;
    s32 GetHeight() const noexcept;
    std::string_view GetName() const noexcept override;

private:
    s32 width;
    s32 height;
};

#endif// LIBTRACKER_CORE_EVENTS_H