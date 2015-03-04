#include "Wander.h"
#include "AITank.h"

Wander::Wander()
{
	mWanderRadius = 0;
	mWanderDistance = 0;
	mJitter = 0;
	mPrevTarget = glm::vec2(0, 0);
}

glm::vec2 Wander::GetForce()
{
	glm::vec2 randTarget = GetRandomTarget();
	glm::vec2 secondTarget = GetRandomTarget();
	glm::vec2 result = randTarget + secondTarget;
	glm::normalize(result);
	result *= mWanderRadius;
	result += owner->mVelocity * mWanderDistance;
	return owner->mVelocity *= 10000;
}

glm::vec2 Wander::GetRandomTarget()
{
	glm::vec2 wanderCirclePos = owner->mPosition + mWanderDistance;
	float x = wanderCirclePos.x + mWanderRadius * cosf((rand() % 360) * RADIAN_CONVERSION);
	float y = wanderCirclePos.y + mWanderRadius * sinf((rand() % 360) * RADIAN_CONVERSION);
	return glm::vec2(x,y);
}