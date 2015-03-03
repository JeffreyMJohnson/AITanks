#ifndef _AI_TANK_H_
#define _AI_TANK_H_

#include "Tank.h"
#include "Globals.h"
#include "SteeringBehaviour.h"
#include <algorithm>


class AITank : public Tank
{
public:
	glm::vec2 mVelocity;
	float mMaxVelocity;
	SteeringBehaviour* mBehaviour;
	float mVisibilityRadius;
	float mWaitTimer;

	AITank() : Tank()
	{
		mVelocity = glm::vec2(0, 0);
		mMaxVelocity = 1;
		mBehaviour = nullptr;
		mVisibilityRadius = 0;
		mWaitTimer = 0;
	}

	AITank(glm::vec2 a_size, glm::vec2 a_position) : Tank(a_size, a_position)
	{
		mVelocity = glm::vec2(0, 0);
		mMaxVelocity = 1;
		mBehaviour = nullptr;
		mVisibilityRadius = 0;
		mWaitTimer = 0;
	}
	
	void Update(float deltaTime)
	{
		//if (dynamic_cast<Seek*>(mBehaviour) != nullptr)
		//{
		//	if (dynamic_cast<Seek*>(mBehaviour)->mIsTagged)
		//	{
		//		if (mWaitTimer > 3)
		//		{
		//			mWaitTimer = 0;
		//			dynamic_cast<Seek*>(mBehaviour)->mIsTagged = false;
		//		}
		//		else
		//		{
		//			mWaitTimer += deltaTime;
		//			return;
		//		}
		//	}
		//}
		mVelocity = mBehaviour->GetForce() * deltaTime;
		float magnitude = glm::length(mVelocity);
		if (mVelocity != glm::vec2(0,0))
			mVelocity = glm::normalize(mVelocity) * std::min(magnitude, mMaxVelocity);
		mPosition += mVelocity * deltaTime;
	}
};

#endif