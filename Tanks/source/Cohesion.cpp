#include "Cohesion.h"

Cohesion::Cohesion()
{
	mNeighborCount = 0;
	mNeighborRadius = 100.0f;
}

Cohesion::Cohesion(std::vector<AITank*>* tankList)
{
	mTankList = tankList;
	Cohesion();
}

glm::vec2 Cohesion::GetForce()
{
	glm::vec2 force = glm::vec2(0, 0);
	for (auto tank : *mTankList)
	{
		if (tank != owner)
		{
			if (glm::distance(owner->mPosition, tank->mPosition) < mNeighborRadius)
			{
				force += tank->mPosition;
				mNeighborCount++;
			}
		}
	}
	if (mNeighborCount == 0)
		return force;
	force /= mNeighborCount;
	force = force - owner->mPosition;
	return glm::normalize(force);

}