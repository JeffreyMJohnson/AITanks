#include "StateTank.h"
#include "StateManager.h"
#include "GoToResource.h"


unsigned int StateTank::mTotalResourceQuantity = 0.0f;

void StateTank::Initialize(Framework* framework, Grid* grid)
{
	mGrid = grid;
	//state manager has ownership of GoToResource heap pointer
	mStateManager = new StateManager(this, new GoToResource);
	Tank::Initialize(framework);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, Grid* grid)
{
	mGrid = grid;
	//state manager has ownership of GoToResource heap pointer
	mStateManager = new StateManager(this, new GoToResource);
	Tank::Initialize(framework, position, size);
}

void StateTank::Initialize(Framework* framework, glm::vec2& position, glm::vec2& size, glm::vec4& color, Grid* grid)
{
	mGrid = grid;
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
	mRotationAngle = atan2f(mVelocity.y, mVelocity.x);
	mFramework->RotateSprite(mSpriteId, mRotationAngle);
}

void StateTank::Draw()
{
	mFramework->DrawSprite(mSpriteId, mColor);
}

unsigned int StateTank::GetCurrentResourceQty()
{
	return mCurrentResourcesQuantity;
}

Tile* StateTank::FindClosestBase()
{
	return FindClosest(mGrid->GetBaseTilesList());
}

Tile* StateTank::FindClosestResource()
{
	return FindClosest(mGrid->GetResourceTilesList());
}

Tile* StateTank::FindClosest(const std::vector<Tile*>& tileList)
{
	//find lowest distance
	float lowDx = FLT_MAX;
	Tile* nearestTile = nullptr;
	for (Tile* t : tileList)
	{
		float dx = glm::distance(mPosition, t->mPosition);
		if (dx < lowDx)
		{
			lowDx = dx;
			nearestTile = t;
		}
	}
	return nearestTile;
}

bool SortOnFScore(Tile* lhs, Tile* rhs)
{
	return lhs->mFScore < rhs->mFScore;
}

void StateTank::AStarPathFind(Tile* goal, bool smoothPath)
{
	std::list<Tile*> priorityQ;
	Tile* startTile = mGrid->GetNearestTile(mPosition);
	priorityQ.push_front(startTile);
	startTile->mGScore = 0;
	startTile->mPathParentNode = startTile;

	while (!priorityQ.empty())
	{
		priorityQ.sort(SortOnFScore);
		Tile* current = priorityQ.front();
		priorityQ.pop_front();

		current->mIsVisited = true;
		if (current != startTile && current != goal && current->mIsWalkable)
		{
			current->mColor = MNF::Color::Yellow();
		}

		if (current == goal)
			break;

		for (auto edge : current->mEdges)
		{
			Tile* neighbor = edge->mEnd;
			if (!neighbor->mIsVisited && neighbor->mIsWalkable)
			{
				float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(DISTANCE, neighbor, goal);
				//float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(MANHATTAN, neighbor, goal);
				if (fScore < neighbor->mGScore)
				{
					neighbor->mPathParentNode = current;
					neighbor->mGScore = current->mGScore + neighbor->mWeight;
					neighbor->mFScore = (int)fScore;
					if (std::find(priorityQ.begin(), priorityQ.end(), neighbor) == priorityQ.end())
					{
						priorityQ.push_back(neighbor);
					}
				}
			}
		}

	}
	if (goal->mPathParentNode == nullptr)
	{
		//no solution
		return;
	}

	mPathTileList.push_back(goal);
	Tile* parent = goal->mPathParentNode;
	mPathTileList.insert(mPathTileList.begin(), parent);
	while (parent != startTile)
	{
		parent = parent->mPathParentNode;
		mPathTileList.insert(mPathTileList.begin(), parent);
	}

	if (smoothPath)
	{

		if (mPathTileList.size() < 3)
			return;
		Tile* start = *mPathTileList.begin();
		Tile* end = *(mPathTileList.begin() + 2);

		//std::find(mPathTileList.begin(), mPathTileList.end(), start)
		//std::find(mPathTileList.begin(), mPathTileList.end(), end)
		while (std::find(mPathTileList.begin(), mPathTileList.end(), end) + 1 != mPathTileList.end())
		{
			if (HasStraightLine(start, end))
			{
				//remove node after start
				mPathTileList.erase(std::find(mPathTileList.begin(), mPathTileList.end(), start) + 1);
				if (std::find(mPathTileList.begin(), mPathTileList.end(), end) + 1 != mPathTileList.end())
				{
					end = *(std::find(mPathTileList.begin(), mPathTileList.end(), end) + 1);
				}
			}
			else
			{
				start = *(std::find(mPathTileList.begin(), mPathTileList.end(), start) + 1);
				if (std::find(mPathTileList.begin(), mPathTileList.end(), end) + 1 != mPathTileList.end())
				{
					end = *(std::find(mPathTileList.begin(), mPathTileList.end(), end) + 1);
				}
			}
		}
	}
}

void StateTank::DkPathFind(Tile* goal)
{
	std::list<Tile*> priorityQ;
	Tile* startTile = mGrid->GetNearestTile(mPosition);
	priorityQ.push_front(startTile);
	//set g score to 0 and current tile's parent (N) to itself
	startTile->mGScore = 0;
	startTile->mPathParentNode = startTile;

	while (!priorityQ.empty())
	{
		Tile* current = priorityQ.front();
		priorityQ.pop_front();

		current->mIsVisited = true;

		if (current != startTile && current != goal && current->mIsWalkable && !current->mIsResource)
		{
			current->mColor = MNF::Color::Aqua();
		}

		if (current == goal)
			break;

		for (Edge* edge : current->mEdges)
		{
			Tile* neighbor = edge->mEnd;
			if (!neighbor->mIsVisited && neighbor->mIsWalkable)
			{
				float gscore = current->mGScore + neighbor->mWeight;
				if (gscore < neighbor->mGScore)
				{
					neighbor->mPathParentNode = current;
					neighbor->mGScore = gscore;
					if (std::find(priorityQ.begin(), priorityQ.end(), neighbor) == priorityQ.end())
					{
						priorityQ.push_back(neighbor);
					}
				}
			}
		}

	}

	mPathTileList.push_back(goal);
	Tile* parent = goal->mPathParentNode;
	mPathTileList.insert(mPathTileList.begin(), parent);
	while (parent != startTile)
	{
		if (!parent->mIsResource && !parent->mIsBase)
		{
			parent->mColor = glm::vec4(1, 0, 1, 1);
		}
		parent = parent->mPathParentNode;

		mPathTileList.insert(mPathTileList.begin(), parent);
	}
}

float StateTank::GetHeuristic(HEURISTIC_TYPE type, Tile* node, Tile* nodeTarget)
{
	float result = 0;
	float distanceX;
	float distanceY;
	switch (type)
	{
	case DISTANCE:
		result = glm::distance(node->mPosition, nodeTarget->mPosition);
		break;
	case MANHATTAN:
		distanceX = abs(node->mPosition.x - nodeTarget->mPosition.x);
		distanceY = abs(node->mPosition.y - nodeTarget->mPosition.y);
		result = distanceX + distanceY;
		break;
	}
	return result;

}

bool StateTank::HasStraightLine(Tile* start, Tile* goal)
{
	MNF::Collider::Ray ray(start->mPosition, mGrid->GetRayDirection(start->mPosition, goal->mPosition));
	//need to check every object for collision
	std::vector<Tile*> nodeList = mGrid->GetTilesInLine(ray, goal);
	for (Tile* tile : nodeList)
	{
		//only need to check non walkable objects
		if (!tile->mIsWalkable)
		{
			AABB box = mGrid->GetAABB(tile);
			float enter = 0.0f;
			float exit = 0.0f;
			if (MNF::Collider::RayAABBIntersect(ray, box, enter, exit))
			{
				//if collision true, no straight line
				return false;
			}
		}
	}
	//no collisions found
	return true;
}
