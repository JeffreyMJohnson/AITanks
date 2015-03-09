#include "Pursue.h"
#include "AITank.h"

glm::vec2 Pursue::GetForce()
{
	glm::vec2 force = glm::normalize((target->mPosition + target->mVelocity) - owner->mPosition);
	force *= owner->mMaxVelocity;
	return force - owner->mVelocity;
}