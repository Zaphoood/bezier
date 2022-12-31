#pragma once
#include <SDL.h>

class VisualObject {
public:
	virtual void draw(SDL_Renderer* renderer) = 0;
};