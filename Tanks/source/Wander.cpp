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
	glm::vec2 jitterVec;
	float theta = rand() % 360;

	jitterVec.x = randTarget.x + cosf(theta * RADIAN_CONVERSION) * mJitter;
	jitterVec.y = randTarget.y + sinf(theta * RADIAN_CONVERSION) * mJitter;
	jitterVec = glm::normalize(jitterVec + randTarget) * mWanderRadius;
	jitterVec += owner->mPosition + mWanderDistance;

	if (jitterVec - owner->mPosition == glm::vec2(0, 0))
		return glm::vec2(0, 0);
	glm::vec2 force = glm::normalize(jitterVec - owner->mPosition);
	force *= owner->mMaxVelocity;
	return force - owner->mVelocity;
}

glm::vec2 Wander::GetRandomTarget()
{
	glm::vec2 wanderCirclePos = owner->mPosition + mWanderDistance;
	float x = wanderCirclePos.x + mWanderRadius * cosf((rand() % 360) * RADIAN_CONVERSION);
	float y = wanderCirclePos.y + mWanderRadius * sinf((rand() % 360) * RADIAN_CONVERSION);
	return glm::vec2(x,y);
}