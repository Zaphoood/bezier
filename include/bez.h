#pragma once
#include <iostream>
#include <vector>
#include "visuals.h"
#include "SDL.h"
#include "event.h"
#include "vector2.h"
#include "bez_point.h"
#include "bez_segment.h"

class Bezier : public VisualObject, public EventListener {
private:
	std::vector<BezierPoint> m_bez_points; // The defining points
	std::vector<BezierSegment> m_segments; // Segments between each point
	int m_selPoint = -1; // Which points is currently selected
	int pointSize = 8;
	void renderCurves();
	void drawPoints(SDL_Renderer* renderer);
	void drawSegments(SDL_Renderer* renderer);
public:
	Bezier();
	Bezier(const std::vector<BezierPoint>& points);
	void addPoint(BezierPoint p);
	void addPoint(Vec2 position);
	void removePoint(int index);
	void setPoint(int index, Vec2 position);
	void update() override;
	void updateSegments();
	void forceUpdateSegments();
	void draw(SDL_Renderer* renderer) override;
	void onEvent(SDL_Event* e) override;
};
