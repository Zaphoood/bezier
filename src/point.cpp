#include <iostream>

#include "point.h"
#include "vector2.h"
#include "visuals.h"

const int DEFAULT_ARM_DISTANCE = 30;
const SDL_Color POINT_LINE = {0, 0, 0, SDL_ALPHA_OPAQUE};
const SDL_Color POINT_FILL = {104, 163, 87, SDL_ALPHA_OPAQUE};
const int POINT_SIZE = 12;
const int ARM_POINT_SIZE = 8;

BezierPoint::BezierPoint(Vec2 position, Vec2 arm_l, Vec2 arm_r,
                         PointType pointType)
    : m_position(position), m_leftArm(arm_l), m_rightArm(arm_r),
      m_pointType(pointType) {}

BezierPoint::BezierPoint(Vec2 position, PointType pointType)
    : m_position(position), m_pointType(pointType) {
  m_leftArm = m_position - Vec2f(DEFAULT_ARM_DISTANCE, 0);
  m_rightArm = m_position + Vec2f(DEFAULT_ARM_DISTANCE, 0);
}

BezierPoint::BezierPoint(Vec2 position)
    : BezierPoint::BezierPoint(position, PointType::Straight) {}

void BezierPoint::setPosition(Vec2f position) {
  Vec2f delta = position - m_position;
  m_position = position;
  m_leftArm += delta;
  m_rightArm += delta;
}

Vec2 BezierPoint::getPosition() { return Vec2(m_position); }

void BezierPoint::setLeftArm(Vec2f position) {
  m_leftArm = position;
  switch (m_pointType) {
  case PointType::Symmetric:
    m_rightArm =
        m_position - (m_leftArm - m_position); // Mirror right arm over the node
    break;
  case PointType::Straight: {
    Vec2f leftRel = getLeftArmRelative();
    float distLeft = abs(getLeftArmRelative().size());
    float distRight = abs(getRightArmRelative().size());
    m_rightArm = m_position + (-(leftRel / distLeft) * distRight);

    break;
  }
  case PointType::Cusp:
    // If point type is cusp, the other arm doesn't need to be changed
    break;
  }
}

void BezierPoint::setRightArm(Vec2f position) {
  m_rightArm = position;
  switch (m_pointType) {
  case PointType::Symmetric:
    m_leftArm = m_position -
                (m_rightArm - m_position); // Mirror right arm over the node
    break;
  case PointType::Straight: {
    Vec2f rightRel = getRightArmRelative();
    float distRight = abs(getRightArmRelative().size());
    float distLeft = abs(getLeftArmRelative().size());
    m_leftArm = m_position + (-(rightRel / distRight) * distLeft);

    break;
  }
  case PointType::Cusp:
    // If point type is cusp, the other arm doesn't need to be changed
    break;
  }
}

Vec2f BezierPoint::getLeftArm() { return m_leftArm; }

Vec2f BezierPoint::getRightArm() { return m_rightArm; }

Vec2f BezierPoint::getLeftArmRelative() { return m_leftArm - m_position; }

Vec2f BezierPoint::getRightArmRelative() { return m_rightArm - m_position; }

bool BezierPoint::isSelected() { return selNode || selL || selR; }

SDL_Rect BezierPoint::getNodeRect() {
  return SDL_Rect{(int)m_position.x - POINT_SIZE / 2,
                  (int)m_position.y - POINT_SIZE / 2, POINT_SIZE, POINT_SIZE};
}

SDL_Rect BezierPoint::getLeftArmRect() {
  return SDL_Rect{(int)(m_leftArm.x - (float)ARM_POINT_SIZE / 2),
                  (int)(m_leftArm.y - (float)ARM_POINT_SIZE / 2),
                  ARM_POINT_SIZE, ARM_POINT_SIZE};
}

SDL_Rect BezierPoint::getRightArmRect() {
  return SDL_Rect{(int)(m_rightArm.x - (float)ARM_POINT_SIZE / 2),
                  (int)(m_rightArm.y - (float)ARM_POINT_SIZE / 2),
                  ARM_POINT_SIZE, ARM_POINT_SIZE};
}

void BezierPoint::update() {
  if (selNode || selL || selR) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (selNode) {
      setPosition(Vec2f(mouse_x, mouse_y));
    } else if (selL) {
      setLeftArm(Vec2f(mouse_x, mouse_y));
    } else if (selR) {
      setRightArm(Vec2f(mouse_x, mouse_y));
    }
    changed = true;
  }
}

void BezierPoint::onEvent(SDL_Event *e) {
  SDL_Point mouse_pos;
  SDL_Rect node_rect, left_arm_rect, right_arm_rect;
  switch (e->type) {
  case SDL_MOUSEBUTTONDOWN:
    SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    node_rect = getNodeRect();
    left_arm_rect = getLeftArmRect();
    right_arm_rect = getRightArmRect();
    if (SDL_PointInRect(&mouse_pos, &node_rect)) {
      if (e->button.button == SDL_BUTTON_LEFT) {
        selNode = true;
      } else if (e->button.button == SDL_BUTTON_RIGHT) {
        switch (m_pointType) {
        case PointType::Cusp:
          m_pointType = PointType::Straight;
          break;
        case PointType::Straight:
          m_pointType = PointType::Symmetric;
          break;
        case PointType::Symmetric:
          m_pointType = PointType::Cusp;
          break;
        }
      }
    } else if (SDL_PointInRect(&mouse_pos, &left_arm_rect)) {
      if (e->button.button == SDL_BUTTON_LEFT) {
        selL = true;
      }
    } else if (SDL_PointInRect(&mouse_pos, &right_arm_rect)) {
      if (e->button.button == SDL_BUTTON_LEFT) {
        selR = true;
      }
    }
    break;
  case SDL_MOUSEBUTTONUP:
    selNode = false;
    selL = false;
    selR = false;
    break;
  }
}

bool BezierPoint::hasChanged() {
  bool tmp = changed;
  changed = false;
  return tmp;
}

void BezierPoint::draw(SDL_Renderer *renderer) {
  SDL_Rect node_rect = getNodeRect();
  SDL_Rect left_arm_rect = getLeftArmRect();
  SDL_Rect right_arm_rect = getRightArmRect();

  switch (m_pointType) {
  case PointType::Symmetric:
    // Fill green if selected
    if (selNode) {
      SDL_SetRenderDrawColor(renderer, POINT_FILL.r, POINT_FILL.g, POINT_FILL.b,
                             POINT_FILL.a);
      SDL_RenderFillRect(renderer, &node_rect);
    }

    // Outline
    SDL_SetRenderDrawColor(renderer, POINT_LINE.r, POINT_LINE.g, POINT_LINE.b,
                           POINT_LINE.a);
    SDL_RenderDrawRect(renderer, &node_rect);
    break;
  case PointType::Straight:
    gfx::drawCircle(renderer, m_position.x, m_position.y, POINT_SIZE / 2);
    break;
  case PointType::Cusp:
    float halfPointSize = (float)POINT_SIZE / 2;
    SDL_Point points[3] = {
        SDL_Point{(int)(m_position.x - halfPointSize),
                  (int)(m_position.y + halfPointSize)},
        SDL_Point{(int)(m_position.x + halfPointSize),
                  (int)(m_position.y + halfPointSize)},
        SDL_Point{(int)(m_position.x), (int)(m_position.y - halfPointSize)},
    };
    SDL_RenderDrawLines(renderer, points, 3);
    SDL_RenderDrawLine(renderer, points[2].x, points[2].y, points[0].x,
                       points[0].y);
    break;
  }

  // Left and right handles
  SDL_SetRenderDrawColor(renderer, POINT_LINE.r, POINT_LINE.g, POINT_LINE.b,
                         POINT_LINE.a);
  SDL_RenderFillRect(renderer, &left_arm_rect);
  SDL_RenderFillRect(renderer, &right_arm_rect);

  // Arms connecting left and right handles to main point
  SDL_RenderDrawLine(renderer, (int)m_position.x, (int)m_position.y,
                     (int)m_leftArm.x, (int)m_leftArm.y);
  SDL_RenderDrawLine(renderer, (int)m_position.x, (int)m_position.y,
                     (int)m_rightArm.x, (int)m_rightArm.y);
}
