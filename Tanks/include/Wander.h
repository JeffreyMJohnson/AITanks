#include "SteeringBehaviour.h"

class Wander : public SteeringBehaviour
{
public:
	float mRadius;
	float mDistance;
	int mWanderAngle;
	int mAngleChange;

	Wander();

	glm::vec2 GetForce();

private:
	glm::vec2 GetRandomPointOnCircle(const float radius, const glm::vec2 position);
	void SetAngle(glm::vec2& vector, float value);
};