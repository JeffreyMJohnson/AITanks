#include "WanderTank.h"


void WanderTank::Update(float timeDelta)
{
	InBoundsLogic();
	mSteering->Wander();
	mSteering->Update();
	mFramework->MoveSprite(mSpriteId, mPosition.x, mPosition.y);
	mRotationAngle = atan2f(mVelocity.y, mVelocity.x);
	mFramework->RotateSprite(mSpriteId, mRotationAngle);
}

void WanderTank::Draw()
{
	mFramework->DrawSprite(mSpriteId, mColor);
}