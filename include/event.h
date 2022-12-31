#pragma once
#include "SDL.h"

class EventListener {
public:
	virtual void update() = 0;
	virtual void onEvent(SDL_Event* e) = 0;
};