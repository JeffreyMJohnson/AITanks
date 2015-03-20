#include "StateManager.h"

StateManager::StateManager(StateTank* owner, State* startState)
{
	mOwner = owner;
	mCurrentState = startState;
}

State* StateManager::GetCurrentState()
{
	return mCurrentState;
}

void StateManager::SetCurrentState(State* newState)
{

	/*
	 First we call the Exit() function of the currentState variable, passing in our owner and ourselves
	 Then we call the Enter() function of the state that was passed in as an argument, again passing in our owner variable and ourselves.
	*/

	mCurrentState->Exit(mOwner, this);

	//THIS CLASS HAS OWNERSHIP OF STATE CLASSES ON THE HEAP, MUST DELETE AFTER CALLING EXIT()
	delete mCurrentState;
	newState->Enter(mOwner, this);
	mCurrentState = newState;
}

void StateManager::Update(float timeDelta)
{
	mCurrentState->Update(timeDelta, mOwner, this);
}
