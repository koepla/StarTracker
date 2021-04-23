#pragma once
#include "event.hpp"

/*
*	Base class for the application, should technically be a singleton (TODO)
*	Holds a running boolean and an event handler instance
*	The running boolean's purpose is to give an inheriting class the ability to exit the program
*	event handler is used to set the onevent function for callbacks, will be used for OpenGL callbacks
*/
class app {

protected:
	bool running;
	sta::event_handler ehandler;

public:

	app() { 
		running = true; 
		ehandler.set_callback([this](const sta::event& e) {
			this->onevent(e);
		});
	}

	app(const app&) = delete;

	void run() 
	{ 
		init();

		while (running) 
		{ 
			update(); 
		} 
	}

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void onevent(const sta::event& e) = 0;
};

/*
*	Function decleration that is called in the main function
*	Has to be implemented in main.cpp, returns a pointer to an instance to an inheriting class
*/
extern app* create();

/*
*	main function, calls the create method and then runs the application
*/
int main(int argc, char** argv) {

	app* program = create();
	program->run();
	delete program;
	return 0;
}