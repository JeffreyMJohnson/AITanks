#include "DepositResource.h"
#include "GoToResource.h"

//api from state base class
void DepositResource::Enter(StateTank* agent, StateManager* manager)
{
	assert(agent->mPathTileList.empty());
	agent->mGrid->ResetTiles();
	agent->DkPathFind(agent->FindClosestBase());
}

void DepositResource::Update(float deltaTime, StateTank* agent, StateManager* manager)
{

	/*
	deposit:
	resources dumped -> go to resource
	*/

	if (!agent->mPathTileList.empty())
	{
		Tile* frontTile = agent->mPathTileList.front();
		//going to next tile
		if (glm::distance(agent->mPosition, frontTile->mPosition) > 5.f)
		{
			agent->mSteering->Seek(agent->mPathTileList.front()->mPosition, 5.0f);
			agent->mSteering->Update();

		}
		else//arrived at next tile
		{
			//change color normal if not base or resource
			if (!frontTile->mIsBase && !frontTile->mIsResource)
			{
				frontTile->mColor = MNF::Color::White();
			}

			//pop front tile of list
			agent->mPathTileList.erase(agent->mPathTileList.begin());
		}
	}
	else
	{
		//at the base
		agent->mTotalResourceQuantity += agent->mCurrentResourcesQuantity;
		agent->mCurrentResourcesQuantity = 0;

		//naked 'new' owned by manager, has responsibility of deleting
		manager->SetCurrentState(new GoToResource);

	}

	/*if (glm::distance(agent->mPosition, agent->FindClosestBase()->mPosition) > 2)
	{
		agent->mSteering->Seek(agent->FindClosestBase()->mPosition, 0.0f);
		agent->mSteering->Update();
	}
	else
	{
		agent->mTotalResourceQuantity += agent->mCurrentResourcesQuantity;
		agent->mCurrentResourcesQuantity = 0;

		//naked 'new' owned by manager, has responsibility of deleting
		manager->SetCurrentState(new GoToResource);
	}*/
}

void DepositResource::Exit(StateTank* agent, StateManager* manager)
{

}
