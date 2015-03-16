#include "FlockTank.h"

void FlockTank::Initialize(Framework* framework, std::vector<Entity*>& entityList)
{
	mEntityList = entityList;
	Tank::Initialize(framework);
}


void FlockTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, std::vector<Entity*>& entityList)
{
	mEntityList = entityList;
	Tank::Initialize(framework, position, size);
}

void FlockTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, std::vector<Entity*>& entityList)
{
	mEntityList = entityList;
	Tank::Initialize(framework, position, size, color);
}

void FlockTank::Update(float timeDelta)
{

}

void FlockTank::Draw()
{

}