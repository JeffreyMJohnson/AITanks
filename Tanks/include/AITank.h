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

	AITank() : Tank()
	{
		mVelocity = glm::vec2(0, 0);
		mMaxVelocity = 1;
		mBehaviour = nullptr;
	}

	AITank(glm::vec2 a_size, glm::vec2 a_position) : Tank(a_size, a_position)
	{
		mVelocity = glm::vec2(0, 0);
		mMaxVelocity = 1;
		mBehaviour = nullptr;
	}
	
	void Update(float deltaTime)
	{
		mVelocity = mBehaviour->GetForce() * deltaTime;
		float magnitude = glm::length(mVelocity);
		if (mVelocity != glm::vec2(0,0))
			mVelocity = glm::normalize(mVelocity) * std::min(magnitude, mMaxVelocity);
		mPosition += mVelocity * deltaTime;

		//if (mPosition.x > MNF::Globals::SCREEN_WIDTH) mPosition.x = 1;
		//if (mPosition.x < 0) mPosition.x = MNF::Globals::SCREEN_WIDTH - 1;
		//if (mPosition.y > MNF::Globals::SCREEN_HEIGHT) mPosition.y = 1;
		//if (mPosition.y < 0) mPosition.y = MNF::Globals::SCREEN_HEIGHT - 1;
	}
};

#endif