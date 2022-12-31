#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "point.h"
#include "vector2.h"
#include "visuals.h"

class Segment : public VisualObject {
  private:
    BezierPoint* m_p0;
    BezierPoint* m_p1;
    // The points on the resulting bezier curve
    std::vector<Vec2> m_curve;
    std::vector<Vec2> getDefiningPoints();
    void renderCurve();
    void renderCurveAdaptive();
    void renderCurve(int steps);

  public:
    void updateCurve();
    Segment(BezierPoint* p0, BezierPoint* p1);
    void draw(SDL_Renderer* renderer) override;
};
