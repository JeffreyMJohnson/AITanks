#include "FleeDecision.h"

FleeDecision::FleeDecision(DecisionTank* owner, glm::vec2& target)
{
	mOwner = owner;
	mTarget = target;
}

void FleeDecision::MakeDecision()
{
	mOwner->mSteering->Flee(mTarget);
}