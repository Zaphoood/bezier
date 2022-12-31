#include "curve.h"

BezierCurve::BezierCurve() { }

BezierCurve::BezierCurve(const std::vector<BezierPoint>& points) 
  : m_bez_points(points) {
  forceUpdateSegments();
}

void BezierCurve::addPoint(BezierPoint p) {
  m_bez_points.push_back(p);
  updateSegments();
}

void BezierCurve::addPoint(Vec2 position) {
  addPoint(BezierPoint(position));
}

void BezierCurve::removePoint(int index) {
  m_bez_points.erase(m_bez_points.begin() + index);
}

void BezierCurve::setPoint(int index, Vec2 position) {
  m_bez_points.at(index).setPosition(position);
}

void BezierCurve::update() {
  for (auto &point : m_bez_points) {
    point.update();
    if (point.hasChanged()) {
      updateCurves(); // TODO: Only re-render affected curves
      break;
    }
  }
}

void BezierCurve::updateSegments() {

  // Updates the segments if there isn't the right number of segments
  if (m_bez_points.size() - 1 != m_segments.size()) {
    forceUpdateSegments();
  }
}

void BezierCurve::forceUpdateSegments() {
  // Generate segments between each adjacent pair of points
  // TODO: Only generate new/changed ones instead of generatig all the segments
  m_segments.clear();
  for (std::vector<BezierPoint>::iterator it = m_bez_points.begin(), sec2last = --m_bez_points.end(); it != sec2last; ++it) {
    m_segments.push_back(Segment(&*it, &*(std::next(it))));
  }
}

void BezierCurve::onEvent(SDL_Event* e) {
  bool anyPointClicked = false;
  Vec2 mouse_pos(0, 0);
  for (std::vector<BezierPoint>::iterator it = m_bez_points.begin(); it != m_bez_points.end(); ++it) {
    it->onEvent(e);
    anyPointClicked |= it->isSelected(); // TODO: Find a better way to check if a click was *not* on one of the points
  }
  // If the mouse click didn't select and existing point, create a new point
  if (!anyPointClicked) {
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
      SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
      addPoint(mouse_pos);
    }
  }
}

void BezierCurve::draw(SDL_Renderer* renderer) {
  drawSegments(renderer);
  drawPoints(renderer);
}

void BezierCurve::drawPoints(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  for (auto &point : m_bez_points) {
    point.draw(renderer);
  }
}

void BezierCurve::drawSegments(SDL_Renderer* renderer) {
  for (auto &segment : m_segments) {
    segment.draw(renderer);
  }
}

void BezierCurve::updateCurves() {
  for (auto &segment : m_segments) {
    segment.updateCurve();
  }
}
