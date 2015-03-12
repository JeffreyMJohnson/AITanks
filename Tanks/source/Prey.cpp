#include "Prey.h"

Prey::Prey(Framework* framework, vec position)
{
	mPosition = position;
	mVelocity = vec(1,1);
	mFramework = framework;

	mSteering = new SteeringManager(this);

	mspriteID = mFramework->CreateSprite(WIDTH, HEIGHT, ".\\resources\\textures\\tank.png", true);
	mFramework->SetSpriteUV(mspriteID, .008f, .016f, .121f, .109f);

	mFramework->MoveSprite(mspriteID, mPosition.x, mPosition.y);
	
}

Prey::~Prey()
{
	delete mSteering;
}

void Prey::Update()
{
	//mVelocity = glm::normalize(mVelocity);
	//mVelocity *= MAX_VELOCITY;
	//mVelocity *= (1 / MASS);

	//mVelocity = glm::min(mVelocity, MAX_VELOCITY);

	//mPosition = mPosition + mVelocity;

	mSteering->Wander();
	mSteering->Update();

	mFramework->MoveSprite(mspriteID, mPosition.x, mPosition.y);
}

void Prey::Draw()
{
	mFramework->DrawSprite(mspriteID, mColor);
}

//IBoid interface required methods
glm::vec2& Prey::GetVelocity()
{
	return mVelocity;
}

float Prey::GetMaxVelocity()
{
	return MAX_VELOCITY;
}

glm::vec2& Prey::GetPosition()
{
	return mPosition;
}

float Prey::GetMass()
{
	return MASS;
}