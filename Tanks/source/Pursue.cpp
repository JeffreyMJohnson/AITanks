#include "Pursue.h"
#include "AITank.h"

glm::vec2 Pursue::GetForce()
{
	//use dx as ratio to fix behaviour when target/owner get close
	float dx = glm::distance(target->mPosition, owner->mPosition);
	glm::vec2 force = glm::normalize(((target->mPosition + target->mVelocity) - owner->mPosition) / dx);
	force *= owner->mMaxVelocity;
	return force - owner->mVelocity;
}