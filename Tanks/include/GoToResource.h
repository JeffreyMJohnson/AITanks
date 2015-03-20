#ifndef _GO_TO_RESOURCE_H_
#define _GO_TO_RESOURCE_H_

#include "State.h"
#include "StateManager.h"

class GoToResource : public State
{
public:
	//api from state base class
	void Enter(StateTank* agent, StateManager* manager);
	void Update(float deltaTime, StateTank* agent, StateManager* manager);
	void Exit(StateTank* agent, StateManager* manager);

};

#endif