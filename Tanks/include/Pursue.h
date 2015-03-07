#ifndef _PURSUE_H_
#define _PURSUE_H_

#include "SteeringBehaviour.h"

class Pursue : public SteeringBehaviour
{
public:
	AITank* target;
	bool mIsTagged = false;

	glm::vec2 GetForce();
};

#endif