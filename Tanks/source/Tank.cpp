#include "Tank.h"

Tank::~Tank()
{
	delete mSteering;
}

void Tank::Initialize(Framework* framework)
{
	mSteering = new SteeringManager(this);
	Entity::Initialize(framework);

}

void Tank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size)
{
	mSteering = new SteeringManager(this);
	Entity::Initialize(framework, position, size);
}

void Tank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color)
{
	mSteering = new SteeringManager(this);
	Entity::Initialize(framework, position, size, color);
}

//IBoid interface methods
glm::vec2& Tank::GetVelocity()
{
	return mVelocity;
}

float Tank::GetMaxVelocity()
{
	return mMaxVelocity;
}

glm::vec2& Tank::GetPosition()
{
	return mPosition;
}

float Tank::GetMass()
{
	return mMass;
}

void Tank::InBoundsLogic()
{
	assert(mBounds != glm::vec4(0, 0, 0, 0));
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
}

