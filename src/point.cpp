#include <iostream>

#include "point.h"
#include "vector2.h"

BezierPoint::BezierPoint(Vec2 position, Vec2 arm_l, Vec2 arm_r, PointType pointType)
  : m_position(position), m_leftArm(arm_l), m_rightArm(arm_r), m_pointType(pointType) {}

BezierPoint::BezierPoint(Vec2 position, PointType pointType)
  : m_position(position), m_pointType(pointType) {
    m_leftArm  = m_position - Vec2f(30, 0);
    m_rightArm = m_position + Vec2f(30, 0);
  }

BezierPoint::BezierPoint(Vec2 position) : BezierPoint::BezierPoint(position, PointType::Straight) {}

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
      m_rightArm = m_position - (m_leftArm - m_position); // Mirror right arm over the node
      break;
    case PointType::Straight:
      {
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
      m_leftArm = m_position - (m_rightArm - m_position); // Mirror right arm over the node
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
  return SDL_Rect{ (int) m_position.x - pointSize / 2, (int) m_position.y - pointSize / 2, pointSize, pointSize };
}

SDL_Rect BezierPoint::getLeftArmRect() {
  return SDL_Rect{ (int) (m_leftArm.x - (float) armPointSize / 2), (int) (m_leftArm.y - (float) armPointSize / 2), armPointSize, armPointSize };
}

SDL_Rect BezierPoint::getRightArmRect() {
  return SDL_Rect{ (int)(m_rightArm.x - (float) armPointSize / 2), (int)(m_rightArm.y - (float) armPointSize / 2), armPointSize, armPointSize };
}

void BezierPoint::update() {
  if (selNode || selL || selR) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (selNode) {
      setPosition(Vec2f(mouse_x, mouse_y));
    }
    else if (selL) {
      setLeftArm(Vec2f(mouse_x, mouse_y));
    }
    else if (selR) {
      setRightArm(Vec2f(mouse_x, mouse_y));
    }
    changed = true;
  }
}

void BezierPoint::onEvent(SDL_Event* e) {
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
      }
      else if (SDL_PointInRect(&mouse_pos, &left_arm_rect)) {
        if (e->button.button == SDL_BUTTON_LEFT) {
          selL = true;
        }
      }
      else if (SDL_PointInRect(&mouse_pos, &right_arm_rect)) {
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
  bool c = changed;
  changed = false;
  return c;
}

void BezierPoint::draw(SDL_Renderer* renderer) {
  SDL_Rect node_rect = getNodeRect();
  SDL_Rect left_arm_rect = getLeftArmRect();
  SDL_Rect right_arm_rect = getRightArmRect();

  // TODO: Change shape depenging on point type
  // Fill green if selected
  if (selNode) {
    SDL_SetRenderDrawColor(renderer, 104, 163, 87, SDL_ALPHA_OPAQUE);   
    SDL_RenderFillRect(renderer, &node_rect);
  }
  
  // Outline
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawRect(renderer, &node_rect);

  // Left and right handles
  SDL_RenderFillRect(renderer, &left_arm_rect);
  SDL_RenderFillRect(renderer, &right_arm_rect);

  // Arms connecting left and right handles to main point
  SDL_RenderDrawLine(renderer, (int) m_position.x, (int) m_position.y, (int) m_leftArm.x, (int) m_leftArm.y);
  SDL_RenderDrawLine(renderer, (int) m_position.x, (int) m_position.y, (int) m_rightArm.x, (int) m_rightArm.y);
}
