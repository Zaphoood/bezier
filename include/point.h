#pragma once
#include "SDL.h"
#include "vector2.h"
#include "visuals.h"
#include "event.h"

enum class PointType {
	Cusp,
	Straight,
	Symmetric
};

class BezierPoint : public VisualObject, public EventListener
{
private:
	Vec2f m_position;
	Vec2f m_leftArm; // Store the absolute position of the left arm
	Vec2f m_rightArm; // See above
	PointType m_pointType; // What kind of point (cusp, symmetric, etc.)
	bool selNode = false; // Equals true if the node was selected by the cursor (-> point will move accordingly)
	bool selL = false; // Equals true if the right arm was selected by the cursor
	bool selR = false; // See above
	bool changed = false; // Set to true when the position changes, set to false when hasChanged() is called
	int pointSize = 8;
	int armPointSize = 6;
public:
	BezierPoint(Vec2 position, Vec2 arm_l, Vec2 arm_r, PointType pointType);
	BezierPoint(Vec2 position, PointType pointType);
	BezierPoint(Vec2 position);
	void setPosition(Vec2 position);
	void setPosition(Vec2f position);
	Vec2 getPosition();
	void setLeftArm(Vec2 position);
	void setLeftArm(Vec2f position);
	void setRightArm(Vec2 position);
	void setRightArm(Vec2f position);
	Vec2f getLeftArm();
	Vec2f getRightArm();
	Vec2f getLeftArmRelative(); // Return the position of the left arm relative to the position of the node
	Vec2f getRightArmRelative(); // See above
	SDL_Rect getNodeRect();
	SDL_Rect getLeftArmRect();
	SDL_Rect getRightArmRect();
	bool isSelected();

	void update() override;
	void onEvent(SDL_Event* e) override;
	void draw(SDL_Renderer* renderer) override;
	bool hasChanged();
};