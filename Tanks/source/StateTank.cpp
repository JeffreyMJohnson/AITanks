#include "StateTank.h"


void StateTank::Initialize(Framework* framework, Grid* grid)
{
	mGrid = grid;
	Tank::Initialize(framework);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid)
{
	mGrid = grid;
	Tank::Initialize(framework, position, size);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid)
{
	mGrid = grid;
	Tank::Initialize(framework, position, size, color);
}

void StateTank::Update(float timeDelta)
{
	mFramework->MoveSprite(mSpriteId, mPosition.x, mPosition.y);
}

void StateTank::Draw()
{
	mFramework->DrawSprite(mSpriteId, mColor);
}


vec StateTank::FindClosestBase()
{
	return mGrid->GetTile(0, 0)->mPosition;
}

vec StateTank::FindClosestResource()
{
	return mGrid->GetTile(mGrid->GRID_ROWS - 1, mGrid->GRID_COLS - 1)->mPosition;
}