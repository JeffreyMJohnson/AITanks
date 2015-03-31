#ifndef _STEERING_MANAGER_H_
#define _STEERING_MANAGER_H_

#include "Globals.h"
#include "IBoid.h"


#include <vector>

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
	void Separate();
	void Alignment();
	void Cohesion();
	void Update();

	void Reset();

	

private:
	//MAGIC NUMBERS 
	//MAGIC NUMBERS 
	const float WANDER_CIRCLE_DISTANCE = 150.f;
	const float WANDER_CIRCLE_RADIUS = 100.f;
	const float WANDER_JITTER_SCALE = .01f;

	//value of -1 is flag for not being set yet
	vec mWanderTarget = vec(-1, -1);
	/*
	const float CIRCLE_DISTANCE = 50;
	const float CIRCLE_RADIUS = 100;
	const int ANGLE_CHANGE = 10;
	const float WANDER_FORCE_SCALE = .5f;
	
	float mWanderAngle_ = 0.0f;
	*/

	
	vec DoSeek(vec target, float slowingRadius);
	vec DoFlee(vec target);
	vec DoWander();
	vec DoEvade(IBoid& target);
	vec DoPursuit(IBoid& target);
	vec DoSeparate();
	vec DoAlignment();
	vec DoCohesion();

	void SetAngle(vec& vector, float value);

	void Truncate(vec& original, float max);
	vec GetRandomVector();
}; 

#endif