#include <iostream>
#include <memory>
#include <vector>

#include "window.h"
#include "bez.h"

int main(int argc, char* argv[]) {
  std::unique_ptr<Window> window(new Window());
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

	return 0;
}
