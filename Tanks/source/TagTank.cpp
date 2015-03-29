#include "TagTank.h"
#include "StateManager.h"


TagTank::~TagTank()
{
}

void TagTank::Initialize(Framework* framework, TagTank* tagPartner, bool isSeeking, Grid* grid)
{
	this->tagPartner = tagPartner;
	this->isSeeking = isSeeking;
	mGrid = grid;
	Tank::Initialize(framework);

}

void TagTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, TagTank* tagPartner, bool isSeeking, Grid* grid)
{
	this->tagPartner = tagPartner;
	this->isSeeking = isSeeking;
	mGrid = grid;
	Tank::Initialize(framework, position, size);
}

void TagTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, TagTank* tagPartner, bool isSeeking, Grid* grid)
{
	this->tagPartner = tagPartner;
	this->isSeeking = isSeeking;
	mGrid = grid;
	Tank::Initialize(framework, position, size, color);
}

//Entity interface methods
void TagTank::Update(float timeDelta)
{
	InBoundsLogic();
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

		//mSteering->Seek(tagPartner->mPosition);
		mSteering->Pursuit(*tagPartner);
	}
	else
	{
		if (glm::distance(tagPartner->mPosition, mPosition) > VISIBILITY_RADIUS)
		{
			
			mSteering->Wander();
		}
		else
		{
			//mSteering->Flee(tagPartner->mPosition);
			mSteering->Evade(*tagPartner);
		}
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