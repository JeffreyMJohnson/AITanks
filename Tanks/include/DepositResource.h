#ifndef _DEPOSIT_RESOURCE_H_
#define _DEPOSIT_RESOURCE_H_

#include "State.h"
#include "StateManager.h"

class DepositResource : public State
{
public:
	//state api
	void Enter(StateTank* agent, StateManager* manager);
	void Update(float deltaTime, StateTank* agent, StateManager* manager);
	void Exit(StateTank* agent, StateManager* manager);
};

#endif