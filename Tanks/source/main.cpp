#include "Globals.h"
#include "framework/Framework.h"
#include "Grid.h"
#include "TagTank.h"

#include <time.h>
#include <iostream>
#include <algorithm>
#include <assert.h>

typedef std::vector<Tile*>::iterator It;
#define GLM_FORCE_PURE
typedef glm::vec2 vec2;


enum HEURISTIC_TYPE
{
	DISTANCE,
	MANHATTAN,
	DIAGONAL
};

void Initialize();
void Destroy();
void HandleUI();
void CreateTanks();
void AutoRun();
void TankLogic(float deltaTime);

/*
//void LoadGridEdgesDiagonal();
//void LoadGridEdgesOneWay();
//bool SortOnFScore(Tile* lhs, Tile* rhs);
//void ThetaStarPathFind();
//void AStarPathFind(bool smoothPath);
//float GetHeuristic(HEURISTIC_TYPE type, Tile* node, Tile* nodeTarget);
*/


const glm::vec4 WHITE = glm::vec4(1, 1, 1, 1);
const glm::vec4 RED = glm::vec4(1, 0, 0, 1);
const glm::vec4 GREEN = glm::vec4(0, 1, 0, 1);
const glm::vec4 BLUE = glm::vec4(0, 0, 1, 1);
const glm::vec4 BROWN = glm::vec4(0.501, 0.152, 0.039, 1.0f);

Framework frk;
bool quit = false;

Grid grid(&frk);

std::vector<Entity*> tankList;

int main()
{
	srand((unsigned int)time(NULL));

	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Tanks Path Find Demo");
	frk.SetBackgroundColor(1, 1, 1, 1);

	//create grid and tanks
	Initialize();

	do
	{
		frk.ClearScreen();

		grid.Update();
		grid.Draw();
		//TankLogic(frk.GetDeltaTime());
		TankLogic(1 / 30.0f);

		HandleUI();
		//AutoRun();

	} while (frk.UpdateFramework() && !quit);



	frk.Shutdown();
	Destroy();

	return 0;
}

void TankLogic(float deltaTime)
{
	for (auto tank : tankList)
	{
		tank->Update(frk.GetDeltaTime());
		tank->Draw();
	}
}

void Destroy()
{
	for (auto e : tankList)
	{
		delete e;
	}
	tankList.clear();
}

void HandleUI()
{
	if (frk.IsKeyDown(ESC))
	{
		quit = true;
	}
	/*
	double xPos = 0;
	double yPos = 0;
	if (frk.IsMouseButtonDown(MOUSE_BUTTON::LEFT, xPos, yPos) && tank.mGoalNode == nullptr)
	{
		ResetTiles();
		//check if current node is old goal node and change color
		GetNearestTile(tank.mPosition.x, tank.mPosition.y)->mColor = GREEN;
		Tile* t = GetNearestTile(xPos, yPos);
		t->mColor = RED;
		mGoalNode = t;
		//tank.mGoalNode = t;
		AStarPathFind(false);
	}
	*/
}

void AutoRun()
{

	//if (tank.mGoalNode == nullptr)
	//{
	//	ResetTiles();
	//	GetNearestTile(tank.mPosition.x, tank.mPosition.y)->mColor = GREEN;
	//	
	//	Tile* t = nullptr;
	//	while (t == nullptr)
	//	{
	//		t = GetRandomTile();
	//		if (t->mColor == BROWN) t = nullptr;
	//	}
	//	t->mColor = RED;
	//	mGoalNode = t;
	//	AStarPathFind(true);
	//	//ThetaStarPathFind();
	//}
}

bool SortOnFScore(Tile* lhs, Tile* rhs)
{
	return lhs->mFScore < rhs->mFScore;
}

float GetHeuristic(HEURISTIC_TYPE type, Tile* node, Tile* nodeTarget)
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

//void ThetaStarPathFind(Tank& tank)
//{
//	std::list<Tile*> priorityQ;
//	Tile* startTile = grid.GetNearestTile(tank.mPosition);
//	priorityQ.push_front(startTile);
//	startTile->mGScore = 0;
//	startTile->mPathParentNode = startTile;
//
//	while (!priorityQ.empty())
//	{
//		priorityQ.sort(SortOnFScore);
//		Tile* current = priorityQ.front();
//		priorityQ.pop_front();
//
//		current->mIsVisited = true;
//		if (current != startTile && current != mGoalNode && current->mIsWalkable)
//		{
//			current->mColor = glm::vec4(1, 1, 0, 1);
//		}
//
//		if (current == mGoalNode)
//			break;
//		for (auto edge : current->mEdges)
//		{
//			Tile* neighbor = edge->mEnd;
//			if (!neighbor->mIsVisited && neighbor->mIsWalkable)
//			{
//				if (grid.HasStraightLine(current->mPathParentNode, neighbor))
//				{
//					float fScore = current->mPathParentNode->mGScore + glm::distance(current->mPathParentNode->mPosition, neighbor->mPosition) + GetHeuristic(DISTANCE, neighbor, mGoalNode);
//					if (fScore < neighbor->mGScore)
//					{
//						neighbor->mPathParentNode = current->mPathParentNode;
//						neighbor->mGScore = current->mGScore + neighbor->mWeight;
//						neighbor->mFScore = (int)fScore;
//					}
//				}
//				else
//				{
//					float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(DISTANCE, neighbor, mGoalNode);
//					//float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(MANHATTAN, neighbor, mGoalNode);
//					if (fScore < neighbor->mGScore)
//					{
//						neighbor->mPathParentNode = current;
//						neighbor->mGScore = current->mGScore + neighbor->mWeight;
//						neighbor->mFScore = (int)fScore;
//					}
//				}
//				if (std::find(priorityQ.begin(), priorityQ.end(), neighbor) == priorityQ.end() && neighbor->mPathParentNode != nullptr)
//				{
//					priorityQ.push_back(neighbor);
//				}
//			}
//		}
//
//	}
//	if (mGoalNode->mPathParentNode == nullptr)
//	{
//		//no solution
//		return;
//	}
//
//	tank.pathList.push_back(mGoalNode);
//	Tile* parent = mGoalNode->mPathParentNode;
//	tank.pathList.insert(tank.pathList.begin(), parent);
//	while (parent != startTile)
//	{
//		parent = parent->mPathParentNode;
//		tank.pathList.insert(tank.pathList.begin(), parent);
//	}
//}
//
//void AStarPathFind(bool smoothPath, Tank& tank)
//{
//	std::list<Tile*> priorityQ;
//	Tile* startTile = GetNearestTile(tank.mPosition.x, tank.mPosition.y);
//	priorityQ.push_front(startTile);
//	startTile->mGScore = 0;
//	startTile->mPathParentNode = startTile;
//
//	while (!priorityQ.empty())
//	{
//		priorityQ.sort(SortOnFScore);
//		Tile* current = priorityQ.front();
//		priorityQ.pop_front();
//
//		current->mIsVisited = true;
//		if (current != startTile && current != mGoalNode && current->mIsWalkable)
//		{
//			current->mColor = glm::vec4(1, 1, 0, 1);
//		}
//
//		if (current == mGoalNode)
//			break;
//
//		for (auto edge : current->mEdges)
//		{
//			Tile* neighbor = edge->mEnd;
//			if (!neighbor->mIsVisited && neighbor->mIsWalkable)
//			{
//				float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(DISTANCE, neighbor, mGoalNode);
//				//float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(MANHATTAN, neighbor, mGoalNode);
//				if (fScore < neighbor->mGScore)
//				{
//					neighbor->mPathParentNode = current;
//					neighbor->mGScore = current->mGScore + neighbor->mWeight;
//					neighbor->mFScore = (int)fScore;
//					if (std::find(priorityQ.begin(), priorityQ.end(), neighbor) == priorityQ.end())
//					{
//						priorityQ.push_back(neighbor);
//					}
//				}
//			}
//		}
//
//	}
//	if (mGoalNode->mPathParentNode == nullptr)
//	{
//		//no solution
//		return;
//	}
//
//	tank.pathList.push_back(mGoalNode);
//	Tile* parent = mGoalNode->mPathParentNode;
//	tank.pathList.insert(tank.pathList.begin(), parent);
//	while (parent != startTile)
//	{
//		parent = parent->mPathParentNode;
//		tank.pathList.insert(tank.pathList.begin(), parent);
//	}
//
//	if (smoothPath)
//	{
//
//		if (tank.pathList.size() < 3)
//			return;
//		Tile* start = *tank.pathList.begin();
//		Tile* end = *(tank.pathList.begin() + 2);
//
//		//std::find(tank.pathList.begin(), tank.pathList.end(), start)
//		//std::find(tank.pathList.begin(), tank.pathList.end(), end)
//		while (std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1 != tank.pathList.end())
//		{
//			if (HasStraightLine(start, end))
//			{
//				//remove node after start
//				tank.pathList.erase(std::find(tank.pathList.begin(), tank.pathList.end(), start) + 1);
//				if (std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1 != tank.pathList.end())
//				{
//					end = *(std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1);
//				}				
//			}
//			else
//			{
//				start = *(std::find(tank.pathList.begin(), tank.pathList.end(), start) + 1);
//				if (std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1 != tank.pathList.end())
//				{
//					end = *(std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1);
//				}
//			}
//		}
//	}
//}

//std::vector<Tile*> GetTilesInLine(MNF::Collider::Ray& ray, Tile* end)
//{
//	std::vector<Tile*> result;
//	vec2 currentPosition = ray.origin;
//	Tile* currentTile = nullptr;
//
//	while (currentTile != end)
//	{
//		currentPosition += end->mSize * ray.direction;
//		currentTile = GetNearestTile(currentPosition.x, currentPosition.y);
//		if (std::find(result.begin(), result.end(), currentTile) == result.end())
//		{
//			result.push_back(currentTile);
//		}
//		
//	}
//	return result;
//}

//bool HasStraightLine(Tile* start, Tile* goal)
//{
//	Ray ray(start->mPosition, GetRayDirection(start->mPosition, goal->mPosition));
//	//need to check every object for collision
//	std::vector<Tile*> nodeList = GetTilesInLine(ray, goal);
//	for (Tile* tile : nodeList)
//	{
//		//only need to check non walkable objects
//		if (!tile->mIsWalkable)
//		{
//			AABB box = GetAABB(tile);
//			float enter = 0.0f;
//			float exit = 0.0f;
//			if (RayAABBIntersect(ray, box, enter, exit))
//			{
//				//if collision true, no straight line
//				return false;
//			}
//		}
//	}
//	//no collisions found
//	return true;
//}

//AABB GetAABB(Tile* tile)
//{
//	float hHeight = tile->mSize.y * .5f;
//	float hWidth = tile->mSize.x * .5f;
//	return AABB(vec2(tile->mPosition.x - hWidth, tile->mPosition.y - hHeight), vec2(tile->mPosition.x + hWidth, tile->mPosition.y + hHeight));
//}

//MNF::Collider::AABB GetAABB(Tank& tank)
//{
//	float hHeight = tank.mSize.y * .5f;
//	float hWidth = tank.mSize.x * .5f;
//	return MNF::Collider::AABB(vec2(tank.mPosition.x - hWidth, tank.mPosition.y - hHeight), vec2(tank.mPosition.x + hWidth, tank.mPosition.y + hHeight));
//}


void CreateTanks()
{
	TagTank* tom = new TagTank;
	TagTank* jerry = new TagTank;
	tom->Initialize(&frk, grid.GetTile(10, 10)->mPosition, glm::vec2(20, 20), glm::vec4(1, 0, 0, 1), jerry, true);
	tom->SetSpriteId(".\\resources\\textures\\tank.png", glm::vec4(.008f, .016f, .121f, .109f));
	tom->mBounds = grid.gridRect;
	tom->mMaxVelocity = 1.5f;
	jerry->Initialize(&frk, grid.GetTile(15, 15)->mPosition, glm::vec2(20, 20), glm::vec4(0, 1, 0, 1), tom, false);
	jerry->SetSpriteId(".\\resources\\textures\\tank.png", glm::vec4(.008f, .016f, .121f, .109f));
	jerry->mBounds = grid.gridRect;
	jerry->mMaxVelocity = 1.0f;
	tankList.push_back(tom);
	tankList.push_back(jerry);
}

void Initialize()
{
	//this needs to be called after the framework is initialized or exception is popped
	grid.Initialize();

	CreateTanks();
}