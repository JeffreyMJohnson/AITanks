#include "TagTank.h"


TagTank::~TagTank()
{
	delete mSteering;
}

void TagTank::Initialize(Framework* framework, TagTank* tagPartner, bool isSeeking)
{
	this->tagPartner = tagPartner;
	this->isSeeking = isSeeking;
	mSteering = new SteeringManager(this);
	Entity::Initialize(framework);
}

void TagTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, TagTank* tagPartner, bool isSeeking)
{
	this->tagPartner = tagPartner;
	this->isSeeking = isSeeking;
	mSteering = new SteeringManager(this);
	Entity::Initialize(framework, position, size);
}

void TagTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, TagTank* tagPartner, bool isSeeking)
{
	this->tagPartner = tagPartner;
	this->isSeeking = isSeeking;
	mSteering = new SteeringManager(this);
	Entity::Initialize(framework, position, size, color);
}

//IBoid interface methods
glm::vec2& TagTank::GetVelocity()
{
	return mVelocity;
}

float TagTank::GetMaxVelocity()
{
	return mMaxVelocity;
}

glm::vec2& TagTank::GetPosition()
{
	return mPosition;
}

float TagTank::GetMass()
{
	return mMass;
}

//Entity interface methods
void TagTank::Update(float timeDelta)
{
	if (isSeeking)
	{
		mSteering->Seek(tagPartner->mPosition);
	}
	else
	{
		mSteering->Flee(tagPartner->mPosition);
	}
	mSteering->Update();
	mFramework->MoveSprite(mSpriteId, mPosition.x, mPosition.y);
}

void TagTank::Draw()
{
	mFramework->DrawSprite(mSpriteId, mColor);
}