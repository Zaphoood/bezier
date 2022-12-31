#include <iostream>
#include "window.h"

Window::Window(const std::vector<VisualObject *>& v_objects, const std::vector<EventListener*>& event_ls)
	: m_v_objects(v_objects), m_event_ls(event_ls) {
	window = nullptr;
	renderer = nullptr;
}

Window::~Window() {
}

void Window::init(const char* title, int x, int y, int w, int h, bool fullscreen) {

	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (window) {
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		}

		std::cout << "Init successfull\n";
		isRunning = true;
	}
	else {
		std::cout << "Failed init\n";
		isRunning = false;
	}

}

void Window::addVisualObject(VisualObject* v_object) {
	m_v_objects.push_back(v_object);
}
void Window::removeVisualObject(VisualObject* v_object) { // TODO: Needs testing
	for (std::vector<VisualObject*>::iterator it = m_v_objects.begin(); it != m_v_objects.end(); it++) {
		if (*it == v_object) {
			m_v_objects.erase(it);
			break;
		}
	}
}

void Window::addEventListener(EventListener* event_l) {
	m_event_ls.push_back(event_l);
}
void Window::removeEventListener(EventListener* event_l) { // TODO: Needs testing
	for (std::vector<EventListener*>::iterator it = m_event_ls.begin(); it != m_event_ls.end(); it++) {
		if (*it == event_l) {
			m_event_ls.erase(it);
			break;
		}
	}
}

void Window::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			isRunning = false;
		}
		else {
			for (std::vector<EventListener *>::iterator it = m_event_ls.begin(); it != m_event_ls.end(); ++it) {
				(*it)->onEvent(&e);
			}
		}
	}
}

void Window::update() {
	unsigned int t_start = SDL_GetTicks();

	// Update all EventListener instances
	for (std::vector<EventListener*>::iterator it = m_event_ls.begin(); it != m_event_ls.end(); ++it) {
		(*it)->update();
	}

	// Limit FPS
	int frametime = SDL_GetTicks() - t_start;
	if (frametime < minFrametime) {
		SDL_Delay(minFrametime - frametime);
	}
}

void Window::render() {
	// Clear the screen (Fill with white color)
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	
	// Draw instances of VisualObject in m_v_objects onto the screen
	for (std::vector<VisualObject *>::iterator it = m_v_objects.begin(); it != m_v_objects.end(); ++it) {
		(*it)->draw(renderer);
	}

	SDL_RenderPresent(renderer);
}

void Window::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Quit\n";
}

bool Window::running() {
	return isRunning;
}
