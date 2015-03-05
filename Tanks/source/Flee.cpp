#include "Flee.h"
#include "AITank.h"

glm::vec2 Flee::GetForce()
{
	glm::vec2 force;
	if (glm::distance(owner->mPosition, target->mPosition) <= target->mVisibilityRadius)
	{
		//set danger flag
		SeeDanger = true;
		//normalize vector 0 causes exception
		if (owner->mPosition - target->mPosition == glm::vec2(0, 0))
			return glm::vec2(0, 0);
		force = glm::normalize(owner->mPosition - target->mPosition);
		force *= owner->mMaxVelocity;
		return force - owner->mVelocity;
	}
	else
	{
		//set danger flag
		SeeDanger = false;
		return force = glm::vec2(rand() % MNF::Globals::SCREEN_WIDTH, rand() % MNF::Globals::SCREEN_HEIGHT);

	}
}