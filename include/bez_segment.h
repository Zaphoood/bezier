#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "bez_point.h"
#include "vector2.h"
#include "visuals.h"

class BezierSegment : public VisualObject {
private:
	BezierPoint* m_p0;
	BezierPoint* m_p1;
	std::vector<Vec2> m_curve; // The points on the resulting bezier curve
	int m_selPoint = -1; // Which points is currently selected
	int pointSize = 8;
	std::vector<Vec2> getDefiningPoints();
	std::vector<Vec2> renderCurve();
	std::vector<Vec2> renderCurveAdaptive();
	std::vector<Vec2> renderCurve(int steps);
public:
	void updateCurve();
	BezierSegment(BezierPoint* p0, BezierPoint* p1);
	void draw(SDL_Renderer* renderer) override;
};
