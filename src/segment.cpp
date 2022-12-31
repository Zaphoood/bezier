#include "segment.h"

const int MAX_STEP_DISTANCE = 8;

Segment::Segment(BezierPoint* p0, BezierPoint* p1) 
: m_p0(p0), m_p1(p1) {
	updateCurve();
}

void Segment::draw(SDL_Renderer* renderer) {
	// Draw curve
	SDL_SetRenderDrawColor(renderer, 151, 64, 53, SDL_ALPHA_OPAQUE);
	for (std::vector<Vec2>::iterator it = m_curve.begin(); it != std::prev(m_curve.end()); ++it) {
		SDL_RenderDrawLine(renderer, it->x, it->y, std::next(it)->x, std::next(it)->y);
	}
}

void Segment::updateCurve() {
	renderCurve();
}

std::vector<Vec2> Segment::getDefiningPoints() {
	return std::vector<Vec2>{ Vec2(m_p0->getPosition()), Vec2(m_p0->getRightArm()), Vec2(m_p1->getLeftArm()), Vec2(m_p1->getPosition()) };
}

void Segment::renderCurve() {
	renderCurveAdaptive();
}

void Segment::renderCurveAdaptive() {
	// Adjust the number of steps to the maximum distance distance between two points
	int max_distance = 0;
	std::vector<Vec2> points = getDefiningPoints();

	for (std::vector<Vec2>::iterator it = points.begin(); it != std::prev(points.end()); ++it) {
		Vec2 distVec = *it - *std::next(it);
    // Using taxicab distance for a rough estimate of the distance
		max_distance = std::max(abs(distVec.x) + abs(distVec.y), max_distance);
	}

	renderCurve(max_distance / MAX_STEP_DISTANCE);
}

void Segment::renderCurve(int steps) {
	float t;

	m_curve.clear();
	for (int i = 0; i <= steps; i++) {
    t = (float) i / steps;

		std::vector<Vec2> points = getDefiningPoints();
		std::vector<Vec2> points2;
		std::vector<Vec2> points3;
		for (std::vector<Vec2>::iterator it = points.begin(); it != std::prev(points.end()); ++it) {
			points2.push_back(AtoBProg(*it, *std::next(it, 1), t));
		}
		for (std::vector<Vec2>::iterator it = points2.begin(); it != std::prev(points2.end()); ++it) {
			points3.push_back(AtoBProg(*it, *std::next(it, 1), t));
		}
		m_curve.push_back(AtoBProg(points3.at(0), points3.at(1), t));
	}
}
