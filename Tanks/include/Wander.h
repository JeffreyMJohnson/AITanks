#include "SteeringBehaviour.h"

class Wander : public SteeringBehaviour
{
public:
	float mWanderRadius;
	float mWanderDistance;
	float mJitter;
	glm::vec2 mPrevTarget;

	Wander();

	glm::vec2 GetForce();

	glm::vec2 GetRandomTarget();

};