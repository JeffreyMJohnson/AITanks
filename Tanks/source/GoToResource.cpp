#include "GoToResource.h"
#include "CollectResource.h"


//api from state base class
void GoToResource::Enter(StateTank* agent, StateManager* manager)
{

}

void GoToResource::Update(float deltaTime, StateTank* agent, StateManager* manager)
{

	/*
	go to resources:
	reach node -> collect resources
	*/
	if (glm::distance(agent->mPosition, agent->FindClosestResource()) > 2)
	{
		agent->mSteering->Seek(agent->FindClosestResource(), 0.0f);
		agent->mSteering->Update();
	}
	else
	{
		//naked 'new' owned by manager, has responsibility of deleting
		manager->SetCurrentState(new CollectResource);
	}
}

void GoToResource::Exit(StateTank* agent, StateManager* manager)
{

}
