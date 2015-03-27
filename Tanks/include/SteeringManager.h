#ifndef _STEERING_MANAGER_H_
#define _STEERING_MANAGER_H_

#include "Globals.h"
#include "IBoid.h"

typedef glm::vec2 vec;


class SteeringManager
{
public:
	vec mSteering;
	IBoid* mHost;

	SteeringManager(IBoid* host);

	void Seek(vec target, float slowingRadius = 20.0f);
	void Flee(vec target);
	void Wander();
	void Evade(IBoid& target);
	void Pursuit(IBoid& target);

	void Update(float timeDelta);

	void Reset();



private:
	//MAGIC NUMBERS 
	const float WANDER_CIRCLE_DISTANCE = 50.f;
	const float WANDER_CIRCLE_RADIUS = 500.f;
	const float WANDER_JITTER_SCALE = 5.f;

	//value of -1 is flag for not being set yet
	vec mWanderTarget = vec(-1, -1);

	vec DoSeek(vec target, float slowingRadius);
	vec DoFlee(vec target);
	vec DoWander();
	vec DoEvade(IBoid& target);
	vec DoPursuit(IBoid& target);
	void SetAngle(vec& vector, float value);

	void Truncate(vec& original, float max);

	vec GetRandomVector();
};

#endif