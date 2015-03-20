#ifndef _STATE_H_
#define _STATE_H_

#include "StateTank.h"

class StateManager;

class State
{
public:
	virtual void Enter(StateTank* agent, StateManager* manager) = 0;
	virtual void Update(float deltaTime, StateTank* agent, StateManager* manager) = 0;
	virtual void Exit(StateTank* agent, StateManager* manager) = 0;
};
#endif