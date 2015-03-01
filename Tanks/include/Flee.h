#ifndef _FLEE_H_
#define _FLEE_H_

#include "SteeringBehaviour.h"
#include "AITank.h"

class Flee : public SteeringBehaviour
{
public:
	AITank* target;

	glm::vec2 GetForce()
	{
		//normalize vector 0 causes exception
		if (owner->mPosition - target->mPosition == glm::vec2(0, 0))
			return glm::vec2(0, 0);
		glm::vec2 force = glm::normalize(owner->mPosition - target->mPosition);
		force *= owner->mMaxVelocity;
		return force - owner->mVelocity;
	}
};
#endif