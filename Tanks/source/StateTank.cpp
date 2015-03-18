#include "StateTank.h"

unsigned int StateTank::mTotalResourceQuantity = 0.0f;

void StateTank::Initialize(Framework* framework, Grid* grid)
{
	mGrid = grid;
	mBaseTile = mGrid->GetTile(0, 0);
	mBaseTile->mColor = glm::vec4(.824, .129, 0, 1);
	mResourceTile = mGrid->GetTile(mGrid->GRID_ROWS - 1, mGrid->GRID_COLS - 1);
	mResourceTile->mColor = glm::vec4(.035, .392, 0, 1);
	mCurrentState = GO_TO_RESOURCE;
	Tank::Initialize(framework);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid)
{
	mGrid = grid;
	mBaseTile = mGrid->GetTile(0, 0);
	mBaseTile->mColor = glm::vec4(.824, .129, 0, 1);
	mResourceTile = mGrid->GetTile(mGrid->GRID_ROWS - 1, mGrid->GRID_COLS - 1);
	mResourceTile->mColor = glm::vec4(.035, .392, 0, 1);
	mCurrentState = GO_TO_RESOURCE;
	Tank::Initialize(framework, position, size);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid)
{
	mGrid = grid;
	mBaseTile = mGrid->GetTile(0, 0);
	mBaseTile->mColor = glm::vec4(.824, .129, 0, 1);
	mResourceTile = mGrid->GetTile(mGrid->GRID_ROWS - 1, mGrid->GRID_COLS - 1);
	mResourceTile->mColor = glm::vec4(.035, .392, 0, 1);
	mCurrentState = GO_TO_RESOURCE;
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
	switch (mCurrentState)
	{
	case GO_TO_RESOURCE:
		RunGoToResource(timeDelta);
		break;
	case COLLECT_RESOURCE:
		RunCollectResources(timeDelta);
		break;
	case DEPOSIT_RESOURCE:
		RunDepositResources(timeDelta);
		break;
	}
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

void StateTank::RunGoToResource(float deltaTime)
{
	/*
	go to resources:
	reach node -> collect resources
	*/
	if (glm::distance(mPosition, FindClosestResource()) > 2)
	{
		mSteering->Seek(FindClosestResource(), 0.0f);
		mSteering->Update();
	}
	else
	{
		mCurrentState = COLLECT_RESOURCE;
	}
}

void StateTank::RunCollectResources(float deltaTime)
{
	/*
	collect:
	runs out of resource (not full) -> go to resource
	can't carry more (full) -> deposit
	*/
	if (mCollectionTimer < mCollectionSpeed)
	{
		mCollectionTimer += deltaTime;
		return;
	}
	else
	{
		mCollectionTimer = 0.0f;
		mCurrentResourcesQuantity++;
		if (mCurrentResourcesQuantity == mTotalResourcesAllowed)
		{
			mCurrentState = DEPOSIT_RESOURCE;
		}
	}

}

void StateTank::RunDepositResources(float deltaTime)
{
	/*
	deposit:
	resources dumped -> go to resource
	*/

	if (glm::distance(mPosition, FindClosestBase()) > 2)
	{
		mSteering->Seek(FindClosestBase(), 0.0f);
		mSteering->Update();
	}
	else
	{
		mTotalResourceQuantity += mCurrentResourcesQuantity;
		mCurrentResourcesQuantity = 0;
		mCurrentState = GO_TO_RESOURCE;
	}

}