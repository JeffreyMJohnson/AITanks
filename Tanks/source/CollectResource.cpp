
/**/
#include "CollectResource.h"
#include "DepositResource.h"
#include "GoToResource.h"


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

		Tile* resourceTile = agent->mGrid->GetNearestTile(agent->mPosition);
		//should never get here with resource tile empty
		assert(resourceTile->mResourceQty > 0);

		agent->mCurrentResourcesQuantity++;
		resourceTile->mResourceQty--;

		//if resource empty set tile state
		if (resourceTile->mResourceQty == 0)
		{
			resourceTile->mIsResource = false;
			resourceTile->mColor = MNF::Color::White();
			agent->mGrid->RemoveResource(resourceTile);
		}

		if (resourceTile->mIsResource)
		{
			float colorRatio = 1 - (resourceTile->mResourceQty / (float)agent->mGrid->TOTAL_RESOURCE_QTY);
			resourceTile->mColor = glm::vec4(colorRatio, 1, colorRatio, 1);
		}

		//if tank full go deposit
		if (agent->mCurrentResourcesQuantity == agent->mTotalResourcesAllowed)
		{
			//naked 'new' owned by manager, has responsibility of deleting
			manager->SetCurrentState(new DepositResource);
			return;
		}
		//tank not full and resource is empty
		if (!resourceTile->mIsResource)
		{
			//naked 'new' owned by manager, has responsibility of deleting
			manager->SetCurrentState(new GoToResource);
		}


	}
}

void CollectResource::Exit(StateTank* agent, StateManager* manager)
{

}
