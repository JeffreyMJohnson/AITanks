#ifndef _STEERING_BEHAVIOUR_H_
#define _STEERING_BEHAVIOUR_H_

class AITank;

class SteeringBehaviour
{
public:
	AITank* owner;
	virtual glm::vec2 GetForce() = 0;
};

#endif