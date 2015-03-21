#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_


#include "State.h"

/**/
class StateManager
{
public:
	StateManager(StateTank* owner, State* startState);

	State* GetCurrentState();
	void SetCurrentState(State* newState);

	State* GetGlobalState();
	void SetGlobalState(State* newState);

	void Update(float timeDelta);

	StateTank* mOwner;
	State* mCurrentState;
	State* mGlobalState;


private:


};

#endif