#ifndef _COHESION_H_
#define _COHESION_H_
#include "SteeringBehaviour.h"
#include "AITank.h"
#include <vector>

class Cohesion : public SteeringBehaviour
{
public:
	int mNeighborCount;
	float mNeighborRadius;
	std::vector<AITank*>* mTankList;

	Cohesion();
	Cohesion(std::vector<AITank*>* tankList);

	glm::vec2 GetForce();
};


#endif