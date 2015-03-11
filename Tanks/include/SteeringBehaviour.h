#ifndef _STEERING_BEHAVIOUR_H_
#define _STEERING_BEHAVIOUR_H_

#include "Globals.h"

class AITank;

enum STEERING_BEHAVIOUR_TYPE
{
	FLEE,
	SEEK,
	WANDER,
	PURSUE,
	EVADE,
	SEPARATIION,
	ALIGNMENT,
	COHESION
};

class SteeringBehaviour
{
public:
	AITank* owner = nullptr;
	float mWeight = 1;

	virtual glm::vec2 GetForce() = 0;
};

#endif