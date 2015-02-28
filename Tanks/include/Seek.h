#ifndef _SEEK_H_
#define _SEEK_H_

#include "SteeringBehaviour.h"

class Seek : public SteeringBehaviour
{
public:
	AITank* target;

	glm::vec2 GetForce()
	{
		if (target->mPosition - owner->mPosition == glm::vec2(0,0))
			return glm::vec2(0,0);
		glm::vec2 force = glm::normalize(target->mPosition - owner->mPosition);
		force *= owner->mMaxVelocity;
		return force - owner->mVelocity;
	}
};
#endif