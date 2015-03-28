#include "framework/Framework.h"
#include "Grid.h"
#include "TagTank.h"
#include "WanderTank.h"
#include "FlockTank.h"
#include "StateTank.h"

#include <time.h>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>

typedef std::vector<Tile*>::iterator It;
#define GLM_FORCE_PURE
typedef glm::vec2 vec2;

using namespace MNF;

/*
left off:
finished bare bones of flocktank class, enough to instantiate in main. steering class needs to implement flocking behaviours
separate and alignment.
*/



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


//const glm::vec4 WHITE = glm::vec4(1, 1, 1, 1);
//const glm::vec4 RED = glm::vec4(1, 0, 0, 1);
//const glm::vec4 GREEN = glm::vec4(0, 1, 0, 1);
//const glm::vec4 BLUE = glm::vec4(0, 0, 1, 1);
//const glm::vec4 BROWN = glm::vec4(0.501, 0.152, 0.039, 1.0f);

Framework frk;
bool quit = false;

Grid grid(&frk);

std::vector<Entity*> tankList;

int main()
{
	srand((unsigned int)time(NULL));

	frk.Initialize(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, "Tanks Path Find Demo");
	frk.SetBackgroundColor(.071, .20, .376, 1);

	//create grid and tanks
	Initialize();

	do
	{
		frk.ClearScreen();

		grid.Update();
		grid.Draw();
		//TankLogic(frk.GetDeltaTime());
		TankLogic(1 / 30.0f);

		std::string s("Tank: ");
		s += std::to_string(dynamic_cast<StateTank*>(tankList[0])->GetCurrentResourceQty());
		frk.DrawText(s.c_str(), Globals::SCREEN_WIDTH * .5f, Globals::SCREEN_HEIGHT * .9f);

		s = "Total: " + std::to_string(dynamic_cast<StateTank*>(tankList[0])->mTotalResourceQuantity);
		frk.DrawText(s.c_str(), Globals::SCREEN_WIDTH * .1f, Globals::SCREEN_HEIGHT * .9f);

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
	/*
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
	*/

	/*
	//10 wander tanks
	unsigned int tankSpriteID = frk.CreateSprite(20, 20, ".\\resources\\textures\\tank.png", true);
	for (int i = 0; i < 5; i++)
	{
	WanderTank* w = new WanderTank;
	w->Initialize(&frk, grid.GetRandomTile()->mPosition, glm::vec2(20, 20), glm::vec4(1, 1, 0, 1));
	w->SetSpriteId(tankSpriteID, glm::vec4(.008f, .016f, .121f, .109f));
	w->mBounds = grid.gridRect;
	tankList.push_back(w);
	}
	*/
	/*
	//unsigned int tankSpriteID = frk.CreateSprite(20, 20, ".\\resources\\textures\\tank.png", true);
	for (int i = 0; i < 25; i++)
	{
	vec position = grid.GetRandomTile()->mPosition;
	FlockTank* t = new FlockTank;
	t->Initialize(&frk, position, vec(20, 20), glm::vec4(0, 0, 1, 1), &tankList);
	t->SetSpriteId(tankSpriteID, glm::vec4(.008f, .016f, .121f, .109f));
	t->mBounds = grid.gridRect;
	tankList.push_back(t);
	}
	*/
	unsigned int tankSpriteID = frk.CreateSprite(20, 20, ".\\resources\\textures\\tank.png", true);

	StateTank* t = new StateTank;
	t->Initialize(&frk, grid.GetTile(0, 0)->mPosition, vec(20, 20), &grid);
	t->SetSpriteId(tankSpriteID, glm::vec4(.008f, .016f, .121f, .109f));
	t->mBounds = grid.gridRect;
	t->mMaxVelocity = 5.0f;
	tankList.push_back(t);
}

void Initialize()
{
	//this needs to be called after the framework is initialized or exception is popped
	grid.Initialize();

	CreateTanks();
}