#include "bez.h"

Bezier::Bezier() {
	m_segments = std::vector<BezierSegment>();
}
Bezier::Bezier(const std::vector<BezierPoint>& points) 
	: m_bez_points(points) {
	m_segments = std::vector<BezierSegment>();
	updateSegments();
	//forceUpdateSegments();
}
void Bezier::addPoint(BezierPoint p) {
	m_bez_points.push_back(p);
	updateSegments();
}
void Bezier::addPoint(Vec2 position) {
	addPoint(BezierPoint(position));
}
void Bezier::removePoint(int index) {
	m_bez_points.erase(m_bez_points.begin() + index);
}
void Bezier::setPoint(int index, Vec2 position) {
	m_bez_points.at(index).setPosition(position);
}
void Bezier::update() {
	for (std::vector<BezierPoint>::iterator it = m_bez_points.begin(); it != m_bez_points.end(); ++it) {
		it->update();
		if (it->hasChanged()) {
			renderCurves(); // TODO: Only re-render affected curves
			break; // If one point was changed, there's no need to update the others since there can only be on point changing at a time
		}
	}
}
void Bezier::updateSegments() {
	// Updates the segments if there isn't the right number of segments
	if (m_bez_points.size() - 1 != m_segments.size()) {
		forceUpdateSegments();
	}
}
void Bezier::forceUpdateSegments() {
	// Generate segments between each adjacent pair of points
	// TODO: Only generate new/changed ones instead of generatig all the segments
	m_segments = std::vector<BezierSegment>();
	for (std::vector<BezierPoint>::iterator it = m_bez_points.begin(), sec2last = --m_bez_points.end(); it != sec2last; ++it) {
		m_segments.push_back(BezierSegment(&*it, &*(std::next(it))));
	}
}
void Bezier::onEvent(SDL_Event* e) {
	bool anyPointClicked = false;
	for (std::vector<BezierPoint>::iterator it = m_bez_points.begin(); it != m_bez_points.end(); ++it) {
		it->onEvent(e);
		anyPointClicked = anyPointClicked || it->isSelected(); // TODO: Find a better way to check if a click was *not* on one of the points
	}
	if (!anyPointClicked) {
		// If the mouse click didn't result in an already existing point being selected, create
		// a new point at the position of the mouse cursor
		switch (e->type) {
		case SDL_MOUSEBUTTONDOWN:
		{
			Vec2 mouse_pos;
			SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
			addPoint(mouse_pos);
			break;
		}
		default:
			break;
		}
	}
}

void Bezier::draw(SDL_Renderer* renderer) {
	drawSegments(renderer);
	drawPoints(renderer);
}
void Bezier::drawPoints(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	for (std::vector<BezierPoint>::iterator it = m_bez_points.begin(); it != m_bez_points.end(); ++it) {
		it->draw(renderer);
	}
}
void Bezier::drawSegments(SDL_Renderer* renderer) {
	for (std::vector<BezierSegment>::iterator it = m_segments.begin(); it != m_segments.end(); ++it) {
		it->draw(renderer);
	}
}
void Bezier::renderCurves() {
	for (std::vector<BezierSegment>::iterator it = m_segments.begin(); it != m_segments.end(); ++it) {
		it->updateCurve();
	}
}