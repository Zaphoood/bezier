#pragma once
#include <vector>
#include "visuals.h"
#include "event.h"

class Window {
public:
	Window();
	Window(const std::vector<VisualObject *>& v_objects, const std::vector<EventListener*>& event_ls);

	void init(const char* title, int x, int y, int w, int h, bool fullscreen);
	void addVisualObject(VisualObject* v_object);
	void removeVisualObject(VisualObject* v_object);
	void addEventListener(EventListener* event_l);
	void removeEventListener(EventListener* event_l);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running();

private:
	bool isRunning = false;
	int min_frametime;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	// Objects in this vector will be drawn on screen every frame
	std::vector<VisualObject *> m_v_objects;
	// Objects in this vector listen to events
	std::vector<EventListener *> m_event_ls;
};
