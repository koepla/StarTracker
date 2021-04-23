#pragma once

class app {

protected:
	bool running;

public:

	app() { running = true; }
	app(const app&) = delete;

	void run() 
	{ 
		this->init();

		while (running) 
		{ 
			this->update(); 
		} 
	}

	virtual void init() = 0;
	virtual void update() = 0;
};

extern app* create();

int main(int argc, char** argv) {

	app* program = create();
	program->run();
	delete program;
	return 0;
}