#include "AITank.h"
#include "Seek.h"
#include "Flee.h"
#include "Wander.h"

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
}

AITank::~AITank()
{
	delete mSteeringBehaviourList[SEEK];
	delete mSteeringBehaviourList[FLEE];
	delete mSteeringBehaviourList[WANDER];
}

void AITank::Update(float deltaTime)
{
	if (mCurrentSteeringType == SEEK)
	{
		assert(dynamic_cast<Seek*>(mSteeringBehaviourList[SEEK])->target != nullptr);
	}
	else if (mCurrentSteeringType == FLEE)
	{
		assert(dynamic_cast<Flee*>(mSteeringBehaviourList[FLEE])->target != nullptr);
	}
	
	
	//is seeking and paused?
	if (mCurrentSteeringType == SEEK && dynamic_cast<Seek*>(mSteeringBehaviourList[mCurrentSteeringType])->mIsTagged)
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
			dynamic_cast<Seek*>(mSteeringBehaviourList[mCurrentSteeringType])->mIsTagged = false;
		}
	}


	mVelocity = mSteeringBehaviourList[mCurrentSteeringType]->GetForce() * deltaTime;
	float magnitude = glm::length(mVelocity);
	if (mVelocity != glm::vec2(0, 0))
		mVelocity = glm::normalize(mVelocity) * std::min(magnitude, mMaxVelocity);
	mPosition += mVelocity * deltaTime;
}

void AITank::SetSteeringType(STEERING_BEHAVIOUR_TYPE type)
{
	mCurrentSteeringType = type;
}

STEERING_BEHAVIOUR_TYPE AITank::GetSteeringType()
{
	return mCurrentSteeringType;
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
}

void AITank::InitWander()
{
	Wander* w = dynamic_cast<Wander*>(mSteeringBehaviourList[WANDER]);
	w->mAngleChange = 5;
	w->mRadius = 50;
	w->mDistance = 300;
}