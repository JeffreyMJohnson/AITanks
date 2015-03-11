#include "Flee.h"
#include "AITank.h"
#include <vector>
glm::vec2 Flee::GetForce()
{
	if (owner->mPosition - target->mPosition == glm::vec2(0, 0))
		return glm::vec2(0, 0);
	glm::vec2 force = glm::normalize(owner->mPosition - target->mPosition);
	force *= owner->mMaxVelocity;

	return force - owner->mVelocity;
}

