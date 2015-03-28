#include "StateManager.h"

StateManager::StateManager(StateTank* owner, State* startState)
{
	mOwner = owner;
	SetCurrentState(startState);
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

	//THIS CLASS HAS OWNERSHIP OF STATE CLASSES ON THE HEAP, MUST DELETE
	//This could be the mPreviousState variable to revert state, must check before deleting
	//delete it before changing so no memory leak
	if (mPreviousState != nullptr && newState != mPreviousState)
	{
		delete mPreviousState;
	}

	if (mCurrentState != nullptr)
	{
	mPreviousState = mCurrentState;
	mCurrentState->Exit(mOwner, this);
	}

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

State* StateManager::GetPreviousState()
{
	return mPreviousState;
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
