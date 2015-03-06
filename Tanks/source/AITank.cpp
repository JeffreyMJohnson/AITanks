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
	Seek* seek = nullptr;
	Flee* flee = nullptr;

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
		if (dynamic_cast<Flee*>(mSteeringBehaviourList[FLEE])->target != nullptr)
		{
			if (glm::distance(mPosition, dynamic_cast<Flee*>(mSteeringBehaviourList[FLEE])->target->mPosition) <= mVisibilityRadius)
			{
				SetSteeringType(FLEE);
			}
		}
		break;
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

void AITank::SetIsTagged(bool isTagged)
{
	dynamic_cast<Seek*>(mSteeringBehaviourList[SEEK])->mIsTagged = isTagged;
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

bool AITank::IsCollided(AITank* other)
{
	float hHeight = mSize.y * .5;
	float hWidth = mSize.x * .5;
	AABB box1(glm::vec2(mPosition.x - hWidth, mPosition.y - hHeight), glm::vec2(mPosition.x + hWidth, mPosition.y + hHeight));
	AABB box2(glm::vec2(other->mPosition.x - hWidth, other->mPosition.y - hHeight), glm::vec2(other->mPosition.x + hWidth, other->mPosition.y + hHeight));

	return MNF::Collider::AABB(box1.minPoint, box1.maxPoint, box2.minPoint, box2.maxPoint);
}