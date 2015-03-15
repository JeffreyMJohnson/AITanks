#include "Tank.h"

Tank::~Tank()
{
	delete mSteering;
}



//IBoid interface methods
glm::vec2& Tank::GetVelocity()
{
	return mVelocity;
}

float Tank::GetMaxVelocity()
{
	return mMaxVelocity;
}

glm::vec2& Tank::GetPosition()
{
	return mPosition;
}

float Tank::GetMass()
{
	return mMass;
}