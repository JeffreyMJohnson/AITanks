#include "AITank.h"
#include "Seek.h"
#include "Flee.h"
#include "Wander.h"
#include "Pursue.h"
#include "Evade.h"
#include "Separation.h"
#include "Alignment.h"
#include "Cohesion.h"



AITank::AITank() : Tank()
{
	AITank(glm::vec2(20, 20), glm::vec2(0, 0));
}

AITank::AITank(glm::vec2 a_size, glm::vec2 a_position) : Tank(a_size, a_position)
{
	mVelocity = glm::vec2(0, 0);
	mMaxVelocity = 1;
	mBehaviour = nullptr;
	mVisibilityRadius = 0;
	mWaitTimer = 0;
	mCurrentSteeringType = WANDER;
	LoadSteeringBehaviours();
	InitWander();
	mNeighborhoodRadius = 0;
}

AITank::~AITank()
{
	for (auto behaviour : mSteeringBehaviourList)
	{
		delete behaviour.second;
	}
	mSteeringBehaviourList.clear();
}

void AITank::Update(float deltaTime)
{
	Seek* seek = nullptr;
	Flee* flee = nullptr;
	Pursue* pursue = nullptr;
	Evade* evade = nullptr;


	switch (mCurrentSteeringType)
	{
	case SEEK:
		assert(dynamic_cast<Seek*>(mSteeringBehaviourList[SEEK])->target != nullptr);
		seek = dynamic_cast<Seek*>(mSteeringBehaviourList[SEEK]);
		if (seek->mIsTagged)
		{
			//timer logic
			if (mWaitTimer < SEEK_PAUSE_TIME)
			{
				mWaitTimer += deltaTime;
				return;
			}
			else
			{
				//reset timer and set flag
				mWaitTimer = 0;
				seek->mIsTagged = false;
			}
		}
		if (IsCollided(seek->target))
		{
			AITank* target = seek->target;
			target->SetSteeringType(SEEK);
			target->SetSeekTarget(this);

			SetSteeringType(FLEE);
			SetFleeTarget(target);

			float t = mMaxVelocity;
			mMaxVelocity = target->mMaxVelocity;
			target->mMaxVelocity = t;

			mColor = glm::vec4(1, 0, 0, 1);
			target->mColor = glm::vec4(0, 1, 0, 1);

			target->SetIsTagged(true);
		}

		break;
	case FLEE:
		assert(dynamic_cast<Flee*>(mSteeringBehaviourList[FLEE])->target != nullptr);
		flee = dynamic_cast<Flee*>(mSteeringBehaviourList[FLEE]);

		//is seeker visible
		if (glm::distance(mPosition, flee->target->mPosition) > mVisibilityRadius)
		{
			SetSteeringType(WANDER);
		}

		break;

	case WANDER:
		if (dynamic_cast<Evade*>(mSteeringBehaviourList[EVADE])->target != nullptr)
		{
			if (glm::distance(mPosition, dynamic_cast<Evade*>(mSteeringBehaviourList[EVADE])->target->mPosition) <= mVisibilityRadius)
			{
				SetSteeringType(EVADE);
			}
		}
		break;
	case PURSUE:
		assert(dynamic_cast<Pursue*>(mSteeringBehaviourList[PURSUE])->target != nullptr);
		pursue = dynamic_cast<Pursue*>(mSteeringBehaviourList[PURSUE]);
		if (pursue->mIsTagged)
		{
			//timer logic
			if (mWaitTimer < SEEK_PAUSE_TIME)
			{
				mWaitTimer += deltaTime;
				return;
			}
			else
			{
				//reset timer and set flag
				mWaitTimer = 0;
				pursue->mIsTagged = false;
			}
		}
		if (IsCollided(pursue->target))
		{
			AITank* target = pursue->target;
			target->SetSteeringType(PURSUE);
			target->SetPursueTarget(this);

			SetSteeringType(EVADE);
			SetEvadeTarget(target);

			float t = mMaxVelocity;
			mMaxVelocity = target->mMaxVelocity;
			target->mMaxVelocity = t;

			mColor = glm::vec4(1, 0, 0, 1);
			target->mColor = glm::vec4(0, 1, 0, 1);

			target->SetIsTagged(true);
		}
		break;
	case EVADE:
		assert(dynamic_cast<Evade*>(mSteeringBehaviourList[EVADE])->target != nullptr);
		evade = dynamic_cast<Evade*>(mSteeringBehaviourList[EVADE]);

		//is seeker visible
		if (glm::distance(mPosition, evade->target->mPosition) > mVisibilityRadius)
		{
			SetSteeringType(WANDER);
		}
		break;
	}
	glm::vec2 direction;
	if (mSteeringPriorityList.empty())
	{
		mVelocity = mSteeringBehaviourList[mCurrentSteeringType]->GetForce() * deltaTime;
		direction = (mPosition + (mSteeringBehaviourList[mCurrentSteeringType]->GetForce())) - mPosition;
	}
	else
	{
		mVelocity = GetForce(mSteeringPriorityList) * deltaTime;
		direction = (mPosition + (GetForce(mSteeringPriorityList)) - mPosition);
	}
	
	
	float magnitude = glm::length(mVelocity);
	/*if (mVelocity != glm::vec2(0, 0))
		mVelocity = glm::normalize(mVelocity) * std::min(magnitude, mMaxVelocity);*/
	mPosition += mVelocity * deltaTime;
	mRotation = atan2f(direction.y, direction.x);
}

void AITank::SetSteeringType(STEERING_BEHAVIOUR_TYPE type)
{
	mCurrentSteeringType = type;
}

STEERING_BEHAVIOUR_TYPE AITank::GetSteeringType()
{
	return mCurrentSteeringType;
}

SteeringBehaviour& AITank::GetBehaviour(STEERING_BEHAVIOUR_TYPE type)
{
	return *mSteeringBehaviourList[type];
}

void AITank::SetSeekTarget(AITank* target)
{
	dynamic_cast<Seek*>(mSteeringBehaviourList[SEEK])->target = target;
}

AITank* AITank::GetSeekTarget()
{
	return dynamic_cast<Seek*>(mSteeringBehaviourList[SEEK])->target;
}

void AITank::SetFleeTarget(AITank* target)
{
	dynamic_cast<Flee*>(mSteeringBehaviourList[FLEE])->target = target;
}
AITank* AITank::GetFleeTarget()
{
	return dynamic_cast<Flee*>(mSteeringBehaviourList[FLEE])->target;
}

void AITank::SetIsTagged(bool isTagged)
{
	dynamic_cast<Seek*>(mSteeringBehaviourList[SEEK])->mIsTagged = isTagged;
	dynamic_cast<Pursue*>(mSteeringBehaviourList[PURSUE])->mIsTagged = isTagged;
}

void AITank::SetPursueTarget(AITank* target)
{
	dynamic_cast<Pursue*>(mSteeringBehaviourList[PURSUE])->target = target;
}

void AITank::SetEvadeTarget(AITank* target)
{
	dynamic_cast<Evade*>(mSteeringBehaviourList[EVADE])->target = target;
}

void AITank::SetBehaviourWeight(const STEERING_BEHAVIOUR_TYPE type, const float weight)
{
	mSteeringBehaviourList[type]->mWeight = weight;
}

void AITank::LoadSteeringBehaviours()
{
	Flee* f = new Flee;
	f->owner = this;
	mSteeringBehaviourList[FLEE] = f;

	Seek* s = new Seek;
	s->owner = this;
	mSteeringBehaviourList[SEEK] = s;

	Wander* w = new Wander;
	w->owner = this;
	mSteeringBehaviourList[WANDER] = w;

	Pursue* p = new Pursue;
	p->owner = this;
	mSteeringBehaviourList[PURSUE] = p;

	Evade* e = new Evade;
	e->owner = this;
	mSteeringBehaviourList[EVADE] = e;

	Separation* sep = new Separation;
	sep->owner = this;
	mSteeringBehaviourList[SEPARATIION] = sep;

	Alignment* a = new Alignment;
	a->owner = this;
	mSteeringBehaviourList[ALIGNMENT] = a;

	Cohesion* c = new Cohesion;
	c->owner = this;
	mSteeringBehaviourList[COHESION] = c;
}

void AITank::InitWander()
{
	Wander* w = dynamic_cast<Wander*>(mSteeringBehaviourList[WANDER]);
	w->mAngleChange = 14;
	w->mRadius = 30;
	w->mDistance = 50;
}

bool AITank::IsCollided(AITank* other)
{
	float hHeight = mSize.y * .5;
	float hWidth = mSize.x * .5;
	AABB box1(glm::vec2(mPosition.x - hWidth, mPosition.y - hHeight), glm::vec2(mPosition.x + hWidth, mPosition.y + hHeight));
	AABB box2(glm::vec2(other->mPosition.x - hWidth, other->mPosition.y - hHeight), glm::vec2(other->mPosition.x + hWidth, other->mPosition.y + hHeight));

	return MNF::Collider::AABB(box1.minPoint, box1.maxPoint, box2.minPoint, box2.maxPoint);
}

glm::vec2 AITank::GetForce(const std::list<STEERING_BEHAVIOUR_TYPE>& behaviourList)
{
	glm::vec2 result = glm::vec2(0,0);
	for (auto behaviour : behaviourList)
	{
		result += mSteeringBehaviourList[behaviour]->GetForce() * mSteeringBehaviourList[behaviour]->mWeight;
		if (glm::length(result) > mMaxVelocity)
		{
			result = glm::normalize(result) * mMaxVelocity;
			break;
		}
	}
	return result;
}