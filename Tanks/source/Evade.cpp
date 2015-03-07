#include "Evade.h"
#include "AITank.h"

glm::vec2 Evade::GetForce()
{
	if (owner->mPosition - (target->mPosition + target->mVelocity) == glm::vec2(0, 0))
		return glm::vec2(0, 0);
	glm::vec2 force = glm::normalize(owner->mPosition - (target->mPosition + target->mVelocity));
	force *= owner->mMaxVelocity;
	return force - owner->mVelocity;
}