#pragma once
#include <SDL.h>

class VisualObject {
public:
	virtual void draw(SDL_Renderer* renderer) = 0;
};

namespace gfx {
  void drawCircle(SDL_Renderer *renderer, int32_t centerX, int32_t centerY, int32_t radius);
}
