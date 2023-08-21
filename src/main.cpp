#include <iostream>
#include <memory>
#include <vector>

#include "curve.h"
#include "window.h"

int main(int argc, char *argv[]) {
  Window window;
  window.init("Extendable Bezier Curve", SDL_WINDOWPOS_CENTERED,
              SDL_WINDOWPOS_CENTERED, 600, 400, false);

  BezierCurve bez = BezierCurve();
  bez.addPoint(Vec2(110, 10));
  bez.addPoint(Vec2(300, 80));

  window.addVisualObject(&bez);
  window.addEventListener(&bez);

  while (window.running()) {
    window.handleEvents();
    window.update();
    window.render();
  }
  window.clean();

  return 0;
}
