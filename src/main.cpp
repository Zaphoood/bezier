#include <iostream>
#include <vector>
#include "window.h"
#include "bez.h"

Window* window = nullptr;

int main(int argc, char* argv[]) {
	window = new Window(std::vector<VisualObject *>(), std::vector<EventListener*>());
	window->init("Extendable Bezier Curve", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, false);

	Bezier bez = Bezier();
	bez.addPoint(Vec2(110, 10));
	bez.addPoint(Vec2(300, 80));

	window->addVisualObject(&bez);
	window->addEventListener(&bez);
	
	while (window->running()) {
		window->handleEvents();
		window->update();
		window->render();
	}
	window->clean();

	delete window;
	return 0;
}