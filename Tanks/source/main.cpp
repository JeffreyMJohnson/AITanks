#include "Globals.h"
#include "framework/Framework.h"
#include "Grid.h"
#include "AITank.h"
#include "Seek.h"
#include "Flee.h"
#include "Wander.h"
#include "Separation.h"
#include "Alignment.h"
#include "Cohesion.h"
#include "Prey.h"
#include "Tank2.h"
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

//struct Ray
//{
//	vec2 origin;
//	vec2 direction;
//
//	Ray(vec2 origin, vec2 direction)
//	{
//		this->origin = origin;
//		this->direction = direction;
//	}
//
//
//};
//
//struct Plane
//{
//	vec2 point;
//	vec2 normal;
//
//	Plane(vec2 point, vec2 normal)
//	{
//		this->point = point;
//		this->normal = normal;
//	}
//};

void Initialize();
void LoadGridEdgesDiagonal();
void LoadGridEdgesOneWay();
void Destroy();
void HandleUI();
bool SortOnFScore(Tile* lhs, Tile* rhs);
void ThetaStarPathFind();
void AStarPathFind(bool smoothPath);
void AutoRun();
float GetHeuristic(HEURISTIC_TYPE type, Tile* node, Tile* nodeTarget);
MNF::Collider::AABB GetAABB(Tank& tank);
void TankLogic(float deltaTime);
void CreateTanks();

const glm::vec4 WHITE = glm::vec4(1, 1, 1, 1);
const glm::vec4 RED = glm::vec4(1, 0, 0, 1);
const glm::vec4 GREEN = glm::vec4(0, 1, 0, 1);
const glm::vec4 BLUE = glm::vec4(0, 0, 1, 1);
const glm::vec4 BROWN = glm::vec4(0.501, 0.152, 0.039, 1.0f);

Framework frk;
bool quit = false;

Grid grid(&frk);

std::vector<Entity*> tankList;


//Tank tank(vec2(20,20), vec2(200,75));
//AITank tank1(vec2(20, 20), vec2(0,0));
//AITank tank2(vec2(20, 20), vec2(0, 0));
//AITank tank3(vec2(20, 20), vec2(0, 0));

Tile* mGoalNode = nullptr;



int main()
{
	srand((unsigned int)time(NULL));

	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Tanks Path Find Demo");
	frk.SetBackgroundColor(1, 1, 1, 1);

	Initialize();

	vec2 tilePos = grid.GetTile(10, 10)->mPosition;
	Prey p1(&frk, tilePos);

	/*
	tank1.mSpriteID = frk.CreateSprite(tank1.mSize.x, tank1.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank1.mSpriteID, .008, .016, .121, .109);
	tank2.mSpriteID = frk.CreateSprite(tank2.mSize.x, tank2.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank2.mSpriteID, .008, .016, .121, .109);

	tank3.mSpriteID = frk.CreateSprite(tank2.mSize.x, tank2.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank3.mSpriteID, .008, .016, .121, .109);

	tank1.SetSteeringType(PURSUE);
	tank1.SetPursueTarget(&tank2);
	/*tank1.mSteeringPriorityList.push_back(PURSUE);
	tank1.mSteeringPriorityList.push_back(WANDER);
	tank1.mColor = GREEN;
	tank1.mVisibilityRadius = 100;

	//tank2.SetSteeringType(FLEE);
	//tank2.SetFleeTarget(&tank1);
	tank2.SetSteeringType(EVADE);
	tank2.SetEvadeTarget(&tank1);
	tank2.mColor = RED;
	tank2.mVisibilityRadius = 100;


	tank3.SetSteeringType(WANDER);
	tank3.SetPursueTarget(&tank1);
	tank3.mSteeringPriorityList.push_back(WANDER);
	tank3.mSteeringPriorityList.push_back(PURSUE);
	tank3.SetBehaviourWeight(WANDER, 1);
	tank3.SetBehaviourWeight(PURSUE, 0);


	//debug
	//tank1.mPosition = GetRandomTilePosition();
	//tank2.mPosition = GetRandomTilePosition();
	Tile* t = GetTile(3, 3);
	tank1.mPosition = t->mPosition;
	t = GetTile(3, 10);
	tank2.mPosition = t->mPosition;
	t = GetTile(10,10);
	tank3.mPosition = t->mPosition;
	tank3.mVelocity = vec2(100, 100);

	tank1.mMaxVelocity = 1000;
	tank2.mMaxVelocity = 500;
	tank3.mMaxVelocity = 700;

	tank1.mVelocity = vec2((rand() % (int)tank1.mMaxVelocity) + 1, (rand() % (int)tank1.mMaxVelocity) + 1);
	tank2.mVelocity = vec2((rand() % (int)tank2.mMaxVelocity) + 1, (rand() % (int)tank2.mMaxVelocity) + 1);
	//tank3.mVelocity = vec2((rand() % (int)tank3.mMaxVelocity) + 1, (rand() % (int)tank3.mMaxVelocity) + 1);

	frk.MoveSprite(tank1.mSpriteID, tank1.mPosition.x, tank1.mPosition.y);
	frk.MoveSprite(tank2.mSpriteID, tank2.mPosition.x, tank2.mPosition.y);
	frk.MoveSprite(tank3.mSpriteID, tank3.mPosition.x, tank3.mPosition.y);

	frk.DrawSprite(tank1.mSpriteID, tank1.mColor);
	frk.DrawSprite(tank2.mSpriteID, tank2.mColor);
	frk.DrawSprite(tank3.mSpriteID);
	*/

	do
	{
		frk.ClearScreen();

		grid.Update();
		grid.Draw();



		//TankLogic(frk.GetDeltaTime());
		TankLogic(1 / 30.0f);


		p1.Update();
		p1.Draw();

		//frk.DrawSprite(tank1.mSpriteID, tank1.mColor);
		//frk.DrawSprite(tank2.mSpriteID, tank2.mColor);
		//frk.DrawSprite(tank3.mSpriteID);

		//tank.Update(frk.GetDeltaTime());
		//frk.MoveSprite(tank.mSpriteID, tank.mPosition.x, tank.mPosition.y);
		//frk.DrawSprite(tank.mSpriteID, tank.mColor);

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
		/*
		tank->Update(deltaTime);
		grid.IsOutOfBounds(tank->mPosition, tank->mSize);
		frk.MoveSprite(tank->mSpriteID, tank->mPosition.x, tank->mPosition.y);
		frk.RotateSprite(tank->mSpriteID, tank->mRotation);
		frk.DrawSprite(tank->mSpriteID);
		*/
	}

	/*
	//tank1.Update(deltaTime);
	//tank2.Update(deltaTime);

	//tank3.Update(deltaTime);
	//
	//IsOutOfBounds(tank1);
	//IsOutOfBounds(tank2);

	////AABB tank1Box = GetAABB(tank1);
	////AABB tank2Box = GetAABB(tank2);

	////if (MNF::Collider::AABB(tank1Box.minPoint, tank1Box.maxPoint, tank2Box.minPoint, tank2Box.maxPoint))
	////{
	////	FlipTankBehaviour();
	////}

	//frk.MoveSprite(tank1.mSpriteID, tank1.mPosition.x, tank1.mPosition.y);
	//frk.RotateSprite(tank1.mSpriteID, tank1.mRotation);
	//frk.MoveSprite(tank2.mSpriteID, tank2.mPosition.x, tank2.mPosition.y);
	//frk.RotateSprite(tank2.mSpriteID, tank2.mRotation);
	//frk.MoveSprite(tank3.mSpriteID, tank3.mPosition.x, tank3.mPosition.y);
	//frk.RotateSprite(tank3.mSpriteID, tank3.mRotation);
	*/
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
	//double xPos = 0;
	//double yPos = 0;
	//if (frk.IsMouseButtonDown(MOUSE_BUTTON::LEFT, xPos, yPos) && tank.mGoalNode == nullptr)
	//{
	//	ResetTiles();
	//	//check if current node is old goal node and change color
	//	GetNearestTile(tank.mPosition.x, tank.mPosition.y)->mColor = GREEN;
	//	Tile* t = GetNearestTile(xPos, yPos);
	//	t->mColor = RED;
	//	mGoalNode = t;
	//	//tank.mGoalNode = t;
	//	AStarPathFind(false);
	//}
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

MNF::Collider::AABB GetAABB(Tank& tank)
{
	float hHeight = tank.mSize.y * .5f;
	float hWidth = tank.mSize.x * .5f;
	return MNF::Collider::AABB(vec2(tank.mPosition.x - hWidth, tank.mPosition.y - hHeight), vec2(tank.mPosition.x + hWidth, tank.mPosition.y + hHeight));
}


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

	/*
	Tank2* t = new Tank2(&frk, grid.GetTile(5, 5)->mPosition);

	tankList.push_back(t);

	Tank2* t2 = new Tank2(&frk, grid.GetTile(10, 10)->mPosition);
	tankList.push_back(t2);
	t->SetHunter(*t2);
	t2->SetTarget(*t);
	*/
	/*
	for (int i = 0; i < 10; i++)
	{
	AITank* t = new AITank(glm::vec2(20,20), glm::vec2(100,100));
	t->mSpriteID = frk.CreateSprite(t->mSize.x, t->mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(t->mSpriteID, .008f, .016f, .121f, .109f);
	//dynamic_cast<Separation*>(&t->GetBehaviour(SEPARATIION))->mTankList = &tankList;

	//t->mSteeringPriorityList.push_back(SEPARATIION);
	//t->SetSteeringType(SEPARATIION);
	dynamic_cast<Alignment*>(&t->GetBehaviour(ALIGNMENT))->mTankList = &tankList;
	dynamic_cast<Alignment*>(&t->GetBehaviour(ALIGNMENT))->mWeight = .33F;
	t->mSteeringPriorityList.push_back(ALIGNMENT);
	//t->SetSteeringType(ALIGNMENT);
	t->mNeighborhoodRadius = 100;
	t->mPosition = grid.GetTile(i + 2, 5)->mPosition;
	t->mVelocity = glm::vec2(100, 100);
	t->mMaxVelocity = 100;

	t->mSteeringPriorityList.push_back(COHESION);
	dynamic_cast<Cohesion*>(&t->GetBehaviour(COHESION))->mTankList = &tankList;
	dynamic_cast<Cohesion*>(&t->GetBehaviour(COHESION))->mWeight = .33F;

	t->mSteeringPriorityList.push_back(SEPARATIION);
	dynamic_cast<Separation*>(&t->GetBehaviour(SEPARATIION))->mTankList = &tankList;
	dynamic_cast<Separation*>(&t->GetBehaviour(SEPARATIION))->mWeight = .33F;
	tankList.push_back(t);
	}
	*/
}

void Initialize()
{
	//this needs to be called after the framework is initialized or exception is popped
	grid.Initialize();

	CreateTanks();
}