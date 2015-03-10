#include "Separation.h"
#include "AITank.h"


Separation::Separation()
{
	mRepulsion = 100;
	mTankList = nullptr;
}

Separation::Separation(std::vector<AITank*>* tankList)
{
	mTankList = tankList;
	Separation();
}

glm::vec2 Separation::GetForce()
{
	glm::vec2 force = owner->mVelocity;
	float numNeighbors = 0;
	for (auto tank : *mTankList)
	{

		if (owner != tank)
		{
			glm::vec2 dir = owner->mPosition - tank->mPosition;
			float dx = glm::length(dir);
			if (dx == 0)
			{
				numNeighbors += 1;
				force += mRepulsion;
			}
			else if (dx < owner->mNeighborhoodRadius)
			{
				numNeighbors += 1;
				force += dir / dx * mRepulsion;
			}
		}
	}

	return force / numNeighbors;

}