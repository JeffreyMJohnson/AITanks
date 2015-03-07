#ifndef _EVADE_H
#define _EVADE_H

#include "SteeringBehaviour.h"

class Evade : public SteeringBehaviour
{
public:
	AITank* target;

	glm::vec2 GetForce();

};

#endif