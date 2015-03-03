#ifndef _FLEE_H_
#define _FLEE_H_

#include "SteeringBehaviour.h"

class Flee : public SteeringBehaviour
{
public:
	AITank* target;

	glm::vec2 GetForce();
};
#endif