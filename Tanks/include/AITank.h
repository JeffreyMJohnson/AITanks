#ifndef _AI_TANK_H_
#define _AI_TANK_H_

#include "Tank.h"
#include "SteeringBehaviour.h"


#include <algorithm>
#include <map>


class AITank : public Tank
{
public:
	glm::vec2 mVelocity;
	float mMaxVelocity;
	SteeringBehaviour* mBehaviour;
	float mVisibilityRadius;
	float mWaitTimer;
	
	

	AITank();

	AITank(glm::vec2 a_size, glm::vec2 a_position);

	~AITank();
	
	void Update(float deltaTime);

	void AITank::SetSteeringType(STEERING_BEHAVIOUR_TYPE type);
	STEERING_BEHAVIOUR_TYPE AITank::GetSteeringType();

	void SetSeekTarget(AITank* target);
	AITank* GetSeekTarget();

	void SetFleeTarget(AITank* target);
	AITank* GetFleeTarget();

private:
	STEERING_BEHAVIOUR_TYPE mCurrentSteeringType;
	std::map<STEERING_BEHAVIOUR_TYPE, SteeringBehaviour*> mSteeringBehaviourList;
	const float SEEK_PAUSE_TIME = 5;
	void LoadSteeringBehaviours();
	void InitWander(); 
};

#endif