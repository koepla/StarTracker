#pragma once
#include <functional>

namespace events {

	/*
	*	Enum for every possible event type
	*/
	enum class event_type {

		NONE = 0,
		MOUSE = 1,
		KEYBD = 2
	};

	/*
	*	Base class for events, is purely abstract because there's no implementation for the name (lol)
	*/
	class event {

	protected:
		event_type etype;

	public:

		event() {
			this->etype = event_type::NONE;
		}

		event_type type() const {
			return etype;
		}

		virtual const char* name() const = 0;
	};

	/*
	*	Keyboard Event, inherits publically from event, therefore has to implement the very silly name function
	*	Has pressed boolean and a keycode, 
	*	the pressed keycode indicates if a key is released or pressed
	*/
	class keybd_event : public event {

	public:
		int keycode;
		bool pressed;

		keybd_event() : event() {

			this->keycode = 0;
			this->pressed = false;
		}

		keybd_event(int keycode, bool pressed = true) : keybd_event() {

			this->keycode = keycode;
			this->pressed = pressed;
		}

		bool is_pressed() const {

			return this->pressed;
		}

		virtual const char* name() const override {

			return "[keyboard event] - keycode: " + keycode;
		}
	};

	/*
	*	typedef to simplify the following code a bit
	*/
	typedef std::function<void(const event&)> eventfn;

	/*
	*	Event Handler class, recieves a callback, which is called, whenever
	*	an Event is passed to the call() function
	*/
	class event_handler {

	public:
		eventfn callback;

		event_handler() {

		}

		void set_callback(eventfn callback) {

			this->callback = callback;
		}

		void call(const event& e) {

			this->callback(e);
		}
	};
}

