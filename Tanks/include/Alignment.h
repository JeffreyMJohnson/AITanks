#ifndef _ALIGNMENT_H_
#define _ALIGNMENT_H_
#include "SteeringBehaviour.h"
#include "AITank.h"


class Alignment : public SteeringBehaviour
{
public:
	std::vector<AITank*>* mTankList;
	int mNeighborCount;
	float mNeighborRadius;

	Alignment();
	Alignment(std::vector<AITank*>* tankList);

	glm::vec2 GetForce();

};
#endif