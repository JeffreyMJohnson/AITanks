#include "AITank.h"
#include "Seek.h"
#include "Flee.h"

AITank::AITank() : Tank()
{
	AITank(glm::vec2(20, 20), glm::vec2(0, 0));
}

AITank::AITank(glm::vec2 a_size, glm::vec2 a_position) : Tank(a_size, a_position)
{
	mVelocity = glm::vec2(0, 0);
	mMaxVelocity = 1;
	mBehaviour = nullptr;
	mVisibilityRadius = 0;
	mWaitTimer = 0;
	mCurrentSteeringType = WANDER;
}

void AITank::Update(float deltaTime)
{


	mVelocity = mBehaviour->GetForce() * deltaTime;
	float magnitude = glm::length(mVelocity);
	if (mVelocity != glm::vec2(0, 0))
		mVelocity = glm::normalize(mVelocity) * std::min(magnitude, mMaxVelocity);
	mPosition += mVelocity * deltaTime;
}