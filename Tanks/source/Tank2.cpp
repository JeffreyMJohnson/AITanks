#include "Tank2.h"

Tank2::Tank2(Framework* framework, glm::vec2 position) : Prey(framework, position)
{

}

void Tank2::Update()
{
	if (mHunter != nullptr)
	{
		mSteering->Evade(*mHunter);

	}
	else if (mTarget != nullptr)
	{
		mSteering->Pursuit(*mTarget);
	}
	else
	{
		mSteering->Wander();
	}
	mSteering->Update();
	mFramework->MoveSprite(mspriteID, mPosition.x, mPosition.y);
}

void Tank2::SetHunter(IBoid& hunter)
{
	mHunter = &hunter;
	mTarget = nullptr;
	mColor = glm::vec4(0, 1, 0, 1);
}

void Tank2::SetTarget(IBoid& target)
{
	mTarget = &target;
	mHunter = nullptr;
	mColor = glm::vec4(1, 0, 0, 1);
}