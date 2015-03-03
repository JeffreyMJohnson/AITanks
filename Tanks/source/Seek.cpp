#include "Seek.h"
#include "AITank.h"
#include "Globals.h"

glm::vec2 Seek::GetForce()
{
	if (target->mPosition - owner->mPosition == glm::vec2(0, 0))
		return glm::vec2(0, 0);
	glm::vec2 force = glm::normalize(target->mPosition - owner->mPosition);
	force *= owner->mMaxVelocity;
	return force - owner->mVelocity;
}