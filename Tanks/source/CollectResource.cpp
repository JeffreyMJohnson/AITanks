
/**/
#include "CollectResource.h"
#include "DepositResource.h"


//api from state base class
void CollectResource::Enter(StateTank* agent, StateManager* manager)
{

}

void CollectResource::Update(float deltaTime, StateTank* agent, StateManager* manager)
{


	/*
	collect:
	runs out of resource (not full) -> go to resource
	can't carry more (full) -> deposit
	*/
	if (agent->mCollectionTimer < agent->mCollectionSpeed)
	{
		agent->mCollectionTimer += deltaTime;
		return;
	}
	else
	{
		agent->mCollectionTimer = 0.0f;
		agent->mCurrentResourcesQuantity++;
		if (agent->mCurrentResourcesQuantity == agent->mTotalResourcesAllowed)
		{
			//naked 'new' owned by manager, has responsibility of deleting
			manager->SetCurrentState(new DepositResource);
		}
	}
}

void CollectResource::Exit(StateTank* agent, StateManager* manager)
{

}
