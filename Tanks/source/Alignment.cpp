#include "Alignment.h"

Alignment::Alignment()
{
	mNeighborCount = 0;
	mNeighborRadius = 50.0f;
}

Alignment::Alignment(std::vector<AITank*>* tankList)
{
	mTankList = tankList;
	Alignment();
}

glm::vec2 Alignment::GetForce()
{
	glm::vec2 force = glm::vec2(0, 0);
	for (auto tank : *mTankList)
	{
		if (tank != owner)
		{
			if (glm::distance(owner->mPosition, tank->mPosition) < mNeighborRadius)
			{
				force += tank->mVelocity;
				mNeighborCount++;
			}
		}
	}
	if (mNeighborCount == 0)
		return force;
	force /= mNeighborCount;
	return glm::normalize(force);
}

