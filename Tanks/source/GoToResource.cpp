#include "GoToResource.h"
#include "CollectResource.h"


//api from state base class
void GoToResource::Enter(StateTank* agent, StateManager* manager)
{
	assert(agent->mPathTileList.empty());
	agent->mGrid->ResetTiles();
	agent->AStarPathFind(agent->FindClosestResource());
}

void GoToResource::Update(float deltaTime, StateTank* agent, StateManager* manager)
{

	/*
	go to resources:
	reach node -> collect resources
	*/



	if (!agent->mPathTileList.empty())
	{
		Tile* frontTile = agent->mPathTileList.front();
		//going to next tile
		if (glm::distance(agent->mPosition, frontTile->mPosition) > 1.5)
		{
			agent->mSteering->Seek(agent->mPathTileList.front()->mPosition, 2.f);
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
		//at the resource
		//naked 'new' owned by manager, has responsibility of deleting
		manager->SetCurrentState(new CollectResource);

	}



	/*
	if (glm::distance(agent->mPosition, agent->FindClosestResource()->mPosition) > 2)
	{
	agent->mSteering->Seek(agent->FindClosestResource()->mPosition, 0.0f);
	agent->mSteering->Update();
	}
	else
	{
	//naked 'new' owned by manager, has responsibility of deleting
	manager->SetCurrentState(new CollectResource);
	}
	*/
}

void GoToResource::Exit(StateTank* agent, StateManager* manager)
{

}
