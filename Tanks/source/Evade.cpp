#include "Evade.h"
#include "AITank.h"

glm::vec2 Evade::GetForce()
{
	if (owner->mPosition - (target->mPosition + target->mVelocity) == glm::vec2(0, 0))
		return glm::vec2(0, 0);
	float dx = glm::distance(owner->mPosition, target->mPosition);
	glm::vec2 force;
	if (dx >= 40)
	{
		force = glm::normalize(owner->mPosition - (target->mPosition + target->mVelocity));
	}
	else
	{
		force = glm::normalize(owner->mPosition - target->mPosition);
	}
	
	force *= owner->mMaxVelocity;
	return force - owner->mVelocity;
}