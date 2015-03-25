#include "WanderDecision.h"

WanderDecision::WanderDecision(DecisionTank* owner)
{
	mOwner = owner;
}

void WanderDecision::MakeDecision()
{
	mOwner->mSteering->Wander();
}