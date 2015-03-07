#ifndef _STEERING_BEHAVIOUR_H_
#define _STEERING_BEHAVIOUR_H_

#include "Globals.h"

class AITank;

enum STEERING_BEHAVIOUR_TYPE
{
	FLEE,
	SEEK,
	WANDER,
	PURSUE
};

class SteeringBehaviour
{
public:
	AITank* owner;
	virtual glm::vec2 GetForce() = 0;
};

#endif