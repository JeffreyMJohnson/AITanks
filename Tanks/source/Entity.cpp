#include "Entity.h"

void Entity::Initialize(Framework* framework)
{
	Initialize(framework, glm::vec2(0, 0), glm::vec2(0, 0), glm::vec4(1, 1, 1, 1));
}

void Entity::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size)
{
	Initialize(framework, position, size, glm::vec4(1, 1, 1, 1));
}

void Entity::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color)
{
	mFramework = framework;
	mPosition = position;
	mSize = size;
	mColor = color;
}

void Entity::SetSpriteId(unsigned int spriteId)
{
	mSpriteId = spriteId;
	mFramework->SetSpriteUV(mSpriteId, 0, 0, 1, 1);
}

void Entity::SetSpriteId(unsigned int spriteId, glm::vec4 spriteUV)
{
	mSpriteId = spriteId;
	mFramework->SetSpriteUV(mSpriteId, spriteUV.x, spriteUV.y, spriteUV.z, spriteUV.w);
}

void Entity::SetSpriteId(const char* texturePath)
{

	SetSpriteId(texturePath, glm::vec4(0, 0, 1, 1));
}

void Entity::SetSpriteId(const char* texturePath, glm::vec4 spriteUV)
{
	mSpriteId = mFramework->CreateSprite(mSize.x, mSize.y, texturePath, true);
	mFramework->SetSpriteUV(mSpriteId, spriteUV.x, spriteUV.y, spriteUV.z, spriteUV.w);
}