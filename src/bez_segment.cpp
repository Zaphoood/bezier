#include "bez_segment.h"
//#include <utility>

BezierSegment::BezierSegment(BezierPoint* p0, BezierPoint* p1) 
: m_p0(p0), m_p1(p1) {
	updateCurve();
}
void BezierSegment::draw(SDL_Renderer* renderer) {
	// Draw curve
	SDL_SetRenderDrawColor(renderer, 151, 64, 53, SDL_ALPHA_OPAQUE);
	for (std::vector<Vec2>::iterator it = m_curve.begin(), second_to_last = --m_curve.end();
		it != second_to_last; ++it) {

		SDL_RenderDrawLine(renderer, it->x, it->y, std::next(it, 1)->x, std::next(it, 1)->y);
	}
}
void BezierSegment::updateCurve() {
	// Renders the curve and stores it in m_curve
	m_curve = renderCurve();
}
std::vector<Vec2> BezierSegment::getDefiningPoints() {
	return std::vector<Vec2>{ Vec2(m_p0->getPosition()), Vec2(m_p0->getRightArm()), Vec2(m_p1->getLeftArm()), Vec2(m_p1->getPosition()) };
}
std::vector<Vec2> BezierSegment::renderCurve() {
	return renderCurveAdaptive();
}
std::vector<Vec2> BezierSegment::renderCurveAdaptive() {
	// Adjust the number of steps to the distance between the start and end point
	int dist = 0;
	std::vector<Vec2> points = getDefiningPoints();

	for (std::vector<Vec2>::iterator it = points.begin(), second_to_last = --points.end();
		it != second_to_last; ++it) {
		Vec2 distVec = *it - *std::next(it, 1);
		dist = std::max(abs(distVec.x) + abs(distVec.y), dist); // Using taxicab distance for a rough estimate of the distance
	}
	int n_steps = dist / 8;
	return renderCurve(n_steps);
}
std::vector<Vec2> BezierSegment::renderCurve(int steps) {
	std::vector<Vec2> curve;
	float progr = 0.0;
	float stepSize = (float)(1.0) / steps;
	for (int i = 0; i <= steps; i++) {
		std::vector<Vec2> points = getDefiningPoints();
		std::vector<Vec2> points2;
		std::vector<Vec2> points3;
		for (std::vector<Vec2>::iterator it = points.begin(), second_to_last = --points.end();
			it != second_to_last; ++it) {
			points2.push_back(AtoBProg(*it, *std::next(it, 1), progr));
		}
		for (std::vector<Vec2>::iterator it = points2.begin(), second_to_last = --points2.end();
			it != second_to_last; ++it) {
			points3.push_back(AtoBProg(*it, *std::next(it, 1), progr));
		}
		curve.push_back(AtoBProg(points3.at(0), points3.at(1), progr));
		progr += stepSize;
	}
	return curve;
}