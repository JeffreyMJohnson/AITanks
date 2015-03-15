#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Globals.h"
#include "framework\Framework.h"

/*
Base class of all drawable objects
*/
class Entity
{
public:
	unsigned int mSpriteId;
	glm::vec2 mPosition;
	glm::vec2 mSize;
	glm::vec4 mColor;

	//needs to be virtual so derived class destructors are called.
	virtual ~Entity(){};

	void Initialize(Framework* framework);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size);
	void Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color);

	void SetSpriteId(unsigned int spriteId);
	void Entity::SetSpriteId(unsigned int spriteId, glm::vec4 spriteUV);
	void SetSpriteId(const char* texturePath);
	void SetSpriteId(const char* texturePath, glm::vec4 spriteUV);

	virtual void Update(float timeDelta) = 0;
	virtual void Draw() = 0;

protected:
	Framework* mFramework;
};

#endif