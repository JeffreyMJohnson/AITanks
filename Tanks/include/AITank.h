#ifndef _AI_TANK_H_
#define _AI_TANK_H_

#include "Tank.h"
#include "SteeringBehaviour.h"

#define GLM_FORCE_PURE
#include <algorithm>
#include <map>

struct AABB
{
	glm::vec2 minPoint;
	glm::vec2 maxPoint;

	AABB(glm::vec2 minPoint, glm::vec2 maxPoint)
	{
		this->minPoint = minPoint;
		this->maxPoint = maxPoint;
	}
};


class AITank : public Tank
{
public:
	glm::vec2 mVelocity;
	float mMaxVelocity;
	SteeringBehaviour* mBehaviour;
	float mVisibilityRadius;
	float mWaitTimer;
	std::list<STEERING_BEHAVIOUR_TYPE> mSteeringPriorityList;
	float mNeighborhoodRadius;

	AITank();

	AITank(glm::vec2 a_size, glm::vec2 a_position);

	~AITank();
	
	void Update(float deltaTime);

	void AITank::SetSteeringType(STEERING_BEHAVIOUR_TYPE type);
	STEERING_BEHAVIOUR_TYPE AITank::GetSteeringType();

	SteeringBehaviour& GetBehaviour(STEERING_BEHAVIOUR_TYPE type);

	void SetSeekTarget(AITank* target);
	AITank* GetSeekTarget();

	void SetFleeTarget(AITank* target);
	AITank* GetFleeTarget();

	void SetIsTagged(bool isTagged);

	void SetPursueTarget(AITank* target);
	void SetEvadeTarget(AITank* target);

	void SetBehaviourWeight(const STEERING_BEHAVIOUR_TYPE type, const float weight);


private:
	STEERING_BEHAVIOUR_TYPE mCurrentSteeringType;
	std::map<STEERING_BEHAVIOUR_TYPE, SteeringBehaviour*> mSteeringBehaviourList;
	const float SEEK_PAUSE_TIME = 5;
	void LoadSteeringBehaviours();
	void InitWander(); 
	bool IsCollided(AITank* other);
	glm::vec2 GetForce(const std::list<STEERING_BEHAVIOUR_TYPE>& behaviourList);
};

#endif