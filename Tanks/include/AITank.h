#ifndef _AI_TANK_H_
#define _AI_TANK_H_

#include "Tank.h"
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
	STEERING_BEHAVIOUR_TYPE mCurrentSteeringType;

	AITank();

	AITank(glm::vec2 a_size, glm::vec2 a_position);
	
	void Update(float deltaTime);
};

#endif