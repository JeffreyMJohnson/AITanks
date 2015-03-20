#include "StateTank.h"
#include "StateManager.h"
#include "GoToResource.h"


unsigned int StateTank::mTotalResourceQuantity = 0.0f;

void StateTank::Initialize(Framework* framework, Grid* grid)
{
	mGrid = grid;
	mBaseTile = mGrid->GetTile(0, 0);
	mBaseTile->mColor = glm::vec4(.824, .129, 0, 1);
	mResourceTile = mGrid->GetTile(mGrid->GRID_ROWS - 1, mGrid->GRID_COLS - 1);
	mResourceTile->mColor = glm::vec4(.035, .392, 0, 1);
	//state manager has ownership of GoToResource heap pointer
	mStateManager = new StateManager(this, new GoToResource);
	Tank::Initialize(framework);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid)
{
	mGrid = grid;
	mBaseTile = mGrid->GetTile(0, 0);
	mBaseTile->mColor = glm::vec4(.824, .129, 0, 1);
	mResourceTile = mGrid->GetTile(mGrid->GRID_ROWS - 1, mGrid->GRID_COLS - 1);
	mResourceTile->mColor = glm::vec4(.035, .392, 0, 1);
	//state manager has ownership of GoToResource heap pointer
	mStateManager = new StateManager(this, new GoToResource);
	Tank::Initialize(framework, position, size);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid)
{
	mGrid = grid;
	mBaseTile = mGrid->GetTile(0, 0);
	mBaseTile->mColor = glm::vec4(.824, .129, 0, 1);
	mResourceTile = mGrid->GetTile(mGrid->GRID_ROWS - 1, mGrid->GRID_COLS - 1);
	mResourceTile->mColor = glm::vec4(.035, .392, 0, 1);
	//state manager has ownership of GoToResource heap pointer
	mStateManager = new StateManager(this, new GoToResource);
	Tank::Initialize(framework, position, size, color);
}

void StateTank::Update(float timeDelta)
{
/*transitions:
go to resources:
	reach node -> collect resources

collect:
	runs out of resource (not full) -> go to resource
	can't carry more (full) -> deposit

deposit:
	resources dumped -> go to resource
*/
	mStateManager->Update(timeDelta);
	mFramework->MoveSprite(mSpriteId, mPosition.x, mPosition.y);
}

void StateTank::Draw()
{
	mFramework->DrawSprite(mSpriteId, mColor);
}

unsigned int StateTank::GetCurrentResourceQty()
{
	return mCurrentResourcesQuantity;
}

vec StateTank::FindClosestBase()
{
	return mBaseTile->mPosition;
}

vec StateTank::FindClosestResource()
{
	return mResourceTile->mPosition;
}