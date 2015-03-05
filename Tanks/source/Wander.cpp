#include "Wander.h"
#include "AITank.h"

/*
This algorithm was derived using the article http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-wander--gamedev-1624
I found it much more explanatory than our lecture docs.
*/

Wander::Wander()
{
	mRadius = 0;
	mDistance = 0;
	mWanderAngle = 0.0f;
	mAngleChange = 0;
}

glm::vec2 Wander::GetForce()
{
	//CIRCLE CENTER POSITION
	//calculate center of circle, because it's in front of owner, use the velocity as a guide.
	glm::vec2 circleCenter = owner->mVelocity;
	//normalize into vector with only direction
	circleCenter = glm::normalize(circleCenter);
	//scale vector by the distance the circle is from owner for correct magnitude.
	circleCenter *= mDistance;

	//DISPLACEMENT FORCE - responsible for left/right turn
	//use vector aligned with y axis
	glm::vec2 displacemenet(0, -1);
	//scale it by circle radius
	displacemenet *= mRadius;

	//randomly change vector direction by making it change it's current angle.
	SetAngle(displacemenet, mWanderAngle);

	//change wander angle a bit, so won't have same value next frame
	int r = (rand() % (int)mAngleChange) + 1;
	float s = (mAngleChange * .5);
	mWanderAngle += r - s;

	//RETURN FORCE
	glm::vec2 force = circleCenter + displacemenet;
	return force;
	
}

void Wander::SetAngle(glm::vec2& vector, float value)
{
	float length = glm::length(vector);
	vector.x = cosf(value * RADIAN_CONVERSION) * length;
	vector.y = sinf(value * RADIAN_CONVERSION) * length;
}

glm::vec2 Wander::GetRandomPointOnCircle(const float radius, const glm::vec2 position)
{
	float x = radius * cosf((rand() % 360) * RADIAN_CONVERSION);
	float y = radius * sinf((rand() % 360) * RADIAN_CONVERSION);
	return glm::vec2(x, y) + position;
}

