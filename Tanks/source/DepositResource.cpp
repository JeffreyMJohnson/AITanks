#include "DepositResource.h"
#include "GoToResource.h"

//api from state base class
void DepositResource::Enter(StateTank* agent, StateManager* manager)
{

}

void DepositResource::Update(float deltaTime, StateTank* agent, StateManager* manager)
{

	/*
	deposit:
	resources dumped -> go to resource
	*/

	if (glm::distance(agent->mPosition, agent->FindClosestBase()) > 2)
	{
		agent->mSteering->Seek(agent->FindClosestBase(), 0.0f);
		agent->mSteering->Update();
	}
	else
	{
		agent->mTotalResourceQuantity += agent->mCurrentResourcesQuantity;
		agent->mCurrentResourcesQuantity = 0;

		//naked 'new' owned by manager, has responsibility of deleting
		manager->SetCurrentState(new GoToResource);
	}
}

void DepositResource::Exit(StateTank* agent, StateManager* manager)
{

}
