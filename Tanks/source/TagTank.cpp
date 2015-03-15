#include "TagTank.h"
#include "Grid.h"



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
	assert(mBounds != glm::vec4(0,0,0,0));
	///check if inbounds
	glm::vec2 halfSize = mSize * 0.5f;
	
	if (mPosition.x - halfSize.x < mBounds.x)//west
	{
		mPosition.x = mBounds.z - halfSize.x;
	}
	else if (mPosition.y - halfSize.y < mBounds.y)//south
	{
		mPosition.y = mBounds.w - halfSize.y;
	}
	else if (mPosition.x + halfSize.x > mBounds.z)//east
	{
		mPosition.x = mBounds.x + halfSize.x;
	}
	else if (mPosition.y + halfSize.y > mBounds.w)//north
	{
		mPosition.y = mBounds.y + halfSize.y;
	}

	//timer logic
	if (mIsTagged)
	{
		if (mWaitTimer < SEEK_PAUSE_TIME)
		{
			mWaitTimer += timeDelta;
			return;
		}
		else
		{
			mWaitTimer = 0.0f;
			mIsTagged = false;
		}
	}

	if (isSeeking)
	{
		
		//tag logic
		if (MNF::Collider::AABBCollide(AABB(mPosition - (mSize  * 0.5f), mPosition + (mSize * 0.5f)), 
			AABB(tagPartner->mPosition - (tagPartner->mSize  * 0.5f), tagPartner->mPosition + (tagPartner->mSize * 0.5f))))
		{
			isSeeking = false;
			tagPartner->isSeeking = true;
			float t = mMaxVelocity;
			mMaxVelocity = tagPartner->mMaxVelocity;
			tagPartner->mMaxVelocity = t;

			glm::vec4 c = mColor;
			mColor = tagPartner->mColor;
			tagPartner->mColor = c;

			tagPartner->mIsTagged = true;
			
		}

		mSteering->Seek(tagPartner->mPosition);
	}
	else
	{
		//if (glm::distance(mPosition, tagPartner->mPosition) > VISIBILITY_RADIUS)
		//{
		//	mSteering->Wander();
		//}
		mSteering->Flee(tagPartner->mPosition);
		mSteering->Wander();
	}
	

	mSteering->Update();
	mFramework->MoveSprite(mSpriteId, mPosition.x, mPosition.y);
	mRotationAngle = atan2f(mVelocity.y, mVelocity.x);
	mFramework->RotateSprite(mSpriteId, mRotationAngle);
}

void TagTank::Draw()
{
	mFramework->DrawSprite(mSpriteId, mColor);
}