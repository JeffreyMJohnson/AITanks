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
	assert(newState != nullptr);
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

State* StateManager::GetGlobalState()
{
	return mGlobalState;
}

void StateManager::SetGlobalState(State* newState)
{
	mGlobalState->Exit(mOwner, this);

	//THIS CLASS HAS OWNERSHIP OF STATE CLASSES ON THE HEAP, MUST DELETE AFTER CALLING EXIT()
	delete mGlobalState;
	mCurrentState = newState;
	//can be set to null
	if (mCurrentState != nullptr)
	{
		mCurrentState->Enter(mOwner, this);
	}
}


void StateManager::Update(float timeDelta)
{
	assert(mCurrentState != nullptr);
	mCurrentState->Update(timeDelta, mOwner, this);
	if (mGlobalState != nullptr)
	{
		mGlobalState->Update(timeDelta, mOwner, this);
	}
}
