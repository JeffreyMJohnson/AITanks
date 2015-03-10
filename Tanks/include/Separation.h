#ifndef _SEPARATION_H_
#define _SEPARATION_H_
#include "SteeringBehaviour.h"
#include <vector>


class Separation : public SteeringBehaviour
{
public:
	float mRepulsion;
	std::vector<AITank*>* mTankList;

	Separation();

	Separation(std::vector<AITank*>* tankList);

	glm::vec2 GetForce();

private:
	

};

#endif
