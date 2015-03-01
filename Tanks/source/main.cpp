#include "Globals.h"
#include "framework/Framework.h"
#include "Tile.h"
#include "Tank.h"
#include "Globals.h"
#include "AITank.h"
#include "Seek.h"
#include "Flee.h"

#include <time.h>
#include <iostream>
#include <algorithm>

typedef std::vector<Tile*>::iterator It;
typedef glm::vec2 vec2;


enum HEURISTIC_TYPE
{
	DISTANCE,
	MANHATTAN,
	DIAGONAL
};



struct Ray
{
	vec2 origin;
	vec2 direction;

	Ray(vec2 origin, vec2 direction)
	{
		this->origin = origin;
		this->direction = direction;
	}


};

struct Plane
{
	vec2 point;
	vec2 normal;

	Plane(vec2 point, vec2 normal)
	{
		this->point = point;
		this->normal = normal;
	}
};

struct AABB
{
	vec2 minPoint;
	vec2 maxPoint;

	AABB(vec2 minPoint, vec2 maxPoint)
	{
		this->minPoint = minPoint;
		this->maxPoint = maxPoint;
	}
};


void CreateGrid();
void LoadGridEdges();
void LoadGridEdgesDiagonal();
void LoadGridEdgesOneWay();
void Destroy();
Tile* GetTile(vec2 position);
Tile* GetTile(int a_row, int a_col);
void UpdateTiles();
Tile* GetRandomTile();
vec2 GetRandomTilePosition();
void HandleUI();
Tile* GetNearestTile(float xPos, float yPos);
bool SortOnFScore(Tile* lhs, Tile* rhs);
void ThetaStarPathFind();
void AStarPathFind(bool smoothPath);
bool HasStraightLine(Tile* start, Tile* goal);
void ResetTiles();
void AutoRun();
float GetHeuristic(HEURISTIC_TYPE type, Tile* node, Tile* nodeTarget);
bool RayPlaneIntersect(Ray& ray, Plane& plane, float& t);
bool RayAABBIntersect(Ray& ray, AABB& box, float& enter, float& exit);
AABB GetAABB(Tile* tile);
std::vector<Tile*> GetTilesInLine(Ray& ray, Tile* end);
vec2 GetRayDirection(const vec2& pointA, const vec2& pointB);
void TankLogic(float deltaTime);
bool IsOutOfBounds(AITank& tank);
void FlipTankBehaviour();


const int GRID_ROWS = 25;
const int GRID_COLS = 25;
const glm::vec4 WHITE = glm::vec4(1, 1, 1, 1);
const glm::vec4 RED = glm::vec4(1, 0, 0, 1);
const glm::vec4 GREEN = glm::vec4(0, 1, 0, 1);
const glm::vec4 BLUE = glm::vec4(0, 0, 1, 1);
const glm::vec4 BROWN = glm::vec4(0.501, 0.152, 0.039, 1.0f);

Framework frk;
bool quit = false;
std::vector<Tile*> grid;
unsigned int mTileSpriteID;
glm::vec4 gridRect;

Tank tank(vec2(20,20), vec2(200,75));
AITank tank1(vec2(20, 20), vec2(0,0));
AITank tank2(vec2(20, 20), vec2(0, 0));
Seek* seekBehaviour;
Flee* fleeBehaviour;

Tile* mGoalNode = nullptr;



int main()
{
	srand(time(NULL));
	
	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Tanks Path Find Demo");
	frk.SetBackgroundColor(1, 1, 1, 1);

	CreateGrid();
	
	tank1.mSpriteID = frk.CreateSprite(tank1.mSize.x, tank1.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank1.mSpriteID, .008, .016, .121, .109);
	tank2.mSpriteID = frk.CreateSprite(tank2.mSize.x, tank2.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank2.mSpriteID, .008, .016, .121, .109);

	seekBehaviour = new Seek;
	seekBehaviour->owner = &tank1;
	seekBehaviour->target = &tank2;
	tank1.mBehaviour = seekBehaviour;
	tank1.mColor = GREEN;

	fleeBehaviour = new Flee;
	fleeBehaviour->owner = &tank2;
	fleeBehaviour->target = &tank1;
	tank2.mBehaviour = fleeBehaviour;
	tank2.mColor = RED;

	//debug
	tank1.mPosition = GetRandomTilePosition();
	tank2.mPosition = GetRandomTilePosition();
	//Tile* t = GetNearestTile(400, 350);
	//tank1.mPosition = t->mPosition;
	//t = GetNearestTile(600, 250);
	//tank2.mPosition = t->mPosition;

	tank1.mMaxVelocity = 900;
	tank2.mMaxVelocity = 700;

	tank1.mVelocity = vec2((rand() % (int)tank1.mMaxVelocity) + 1, (rand() % (int)tank1.mMaxVelocity) + 1);
	tank2.mVelocity = vec2((rand() % (int)tank2.mMaxVelocity) + 1, (rand() % (int)tank2.mMaxVelocity) + 1);


	frk.MoveSprite(tank1.mSpriteID, tank1.mPosition.x, tank1.mPosition.y);
	frk.MoveSprite(tank2.mSpriteID, tank2.mPosition.x, tank2.mPosition.y);

	frk.DrawSprite(tank1.mSpriteID);
	frk.DrawSprite(tank2.mSpriteID);

	/*tank.mSpriteID = frk.CreateSprite(tank.mSize.x, tank.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank.mSpriteID, .008, .016, .121, .109);

	Tile* t = GetTile(GetRandomTilePosition());
	tank.mPosition = t->mPosition;
	tank.mLastNodeVisited = t;
	frk.MoveSprite(tank.mSpriteID, tank.mPosition.x, tank.mPosition.y);*/

	do
	{
		frk.ClearScreen();
		UpdateTiles();

		TankLogic(frk.GetDeltaTime());
		
		frk.DrawSprite(tank1.mSpriteID);
		frk.DrawSprite(tank2.mSpriteID);


		/*tank.Update(frk.GetDeltaTime());
		frk.MoveSprite(tank.mSpriteID, tank.mPosition.x, tank.mPosition.y);
		frk.DrawSprite(tank.mSpriteID, tank.mColor);*/

		HandleUI();
		//AutoRun();

	} while (frk.UpdateFramework() && !quit);



	frk.Shutdown();
	Destroy();

	return 0;
}

void TankLogic(float deltaTime)
{
	tank1.Update(deltaTime);
	tank2.Update(deltaTime);

	if (IsOutOfBounds(tank1) || IsOutOfBounds(tank2))
	{
		FlipTankBehaviour();
	}
	frk.MoveSprite(tank1.mSpriteID, tank1.mPosition.x, tank1.mPosition.y);
	frk.MoveSprite(tank2.mSpriteID, tank2.mPosition.x, tank2.mPosition.y);

}

void FlipTankBehaviour()
{
	//tank1 seek, tank2 flee
	if (tank1.mBehaviour == seekBehaviour)
	{
		seekBehaviour->owner = &tank2;
		seekBehaviour->target = &tank1;
		fleeBehaviour->owner = &tank1;
		fleeBehaviour->target = &tank2;
		tank1.mBehaviour = fleeBehaviour;
		tank2.mBehaviour = seekBehaviour;
		tank1.mColor = GREEN;
		tank2.mColor = RED;
	}
	//tank1 flee, tank2 seek
	else
	{
		seekBehaviour->owner = &tank1;
		seekBehaviour->target = &tank2;
		fleeBehaviour->owner = &tank2;
		fleeBehaviour->target = &tank1;
		tank1.mBehaviour = seekBehaviour;
		tank2.mBehaviour = fleeBehaviour;
		tank1.mColor = RED;
		tank2.mColor = GREEN;
	}
}

bool IsOutOfBounds(AITank& tank)
{
	bool result = false;
	if (tank.mPosition.x < gridRect.x)
	{
		tank.mPosition.x = gridRect.z;
		result = true;
	}
	else if (tank.mPosition.x > gridRect.z)
	{
		tank.mPosition.x = gridRect.x;
		result = true;
	}

	if (tank.mPosition.y < gridRect.y)
	{
		tank.mPosition.y = gridRect.w;
		result = true;
	}
	else if (tank.mPosition.y > gridRect.w)
	{
		tank.mPosition.y = gridRect.y;
		result = true;
	}
	return result;
}

Tile* GetNearestTile(float xPos, float yPos)
{
	Tile* result = nullptr;
	float dx = INT_MAX;
	for (auto tile : grid)
	{
		if (glm::distance(tile->mPosition, vec2(xPos, yPos)) < dx)
		{
			result = tile;
			dx = glm::distance(tile->mPosition, vec2(xPos, yPos));
		}
	}
	return result;
}

Tile* GetRandomTile()
{
	return grid[rand() % grid.size()];
}

vec2 GetRandomTilePosition()
{
	return GetRandomTile()->mPosition;
}

void CreateGrid()
{
	const int wallProbability = 0;//int between 0 and 100. greater increases likelyhood of tile being wall
	vec2 tileSize(25, 25);
	mTileSpriteID = frk.CreateSprite(tileSize.x, tileSize.y, ".\\resources\\textures\\Basic.png", true);

	vec2 startPos(200, 75);
	gridRect.x = startPos.x - (tileSize.x * .5);
	gridRect.y = startPos.y - (tileSize.y * .5);
	vec2 position = startPos;
	for (int row = 0; row < GRID_ROWS; row++)
	{
		for (int col = 0; col < GRID_COLS; col++)
		{
			Tile* t = new Tile(row, col);
			t->mSize = tileSize;
			t->mPosition = position;
			grid.push_back(t);
			if (col == GRID_COLS - 1)
			{
				position.x = startPos.x;
			}
			else
			{
				position.x += tileSize.x;
			}
			//see if wall
			if ((rand() % 100) + 1 <= wallProbability)
			{
				t->mColor = BROWN;
				t->mWeight = INT_MAX;
				t->mIsWalkable = false;
			}

		}
		position.y += tileSize.y;
	}
	Tile* t = grid.back();
	gridRect.z = t->mPosition.x + (tileSize.x * .5);
	gridRect.w = t->mPosition.y + (tileSize.y * .5);
	LoadGridEdges();
	//LoadGridEdgesDiagonal();
	//LoadGridEdgesOneWay();
}

void UpdateTiles()
{
	for (auto tile : grid)
	{
		frk.MoveSprite(mTileSpriteID, tile->mPosition.x, tile->mPosition.y);
		frk.DrawSprite(mTileSpriteID, tile->mColor);
	}
}

Tile* GetTile(vec2 position)
{
	for (auto tile : grid)
	{
		if (tile->mPosition == position)
			return tile;
	}
	return nullptr;
}

Tile* GetTile(int a_row, int a_col)
{
	return grid[a_row * GRID_ROWS + a_col];
}

void LoadGridEdges()
{
	for (auto tile : grid)
	{
		//north edge
		if (tile->rowPos + 1 < GRID_ROWS)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos + 1, tile->colPos);
			tile->mEdges.push_back(e);
		}
		//south edge
		if (tile->rowPos - 1 >= 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos - 1, tile->colPos);
			tile->mEdges.push_back(e);
		}
		//east edge
		if (tile->colPos + 1 < GRID_COLS)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos, tile->colPos + 1);
			tile->mEdges.push_back(e);
		}
		//west edge
		if (tile->colPos - 1 >= 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos, tile->colPos - 1);
			tile->mEdges.push_back(e);
		}
	}
}

void LoadGridEdgesOneWay()
{
	for (auto tile : grid)
	{
		//north edge
		if (tile->rowPos + 1 < GRID_ROWS && GetTile(tile->rowPos + 1, tile->colPos)->mEdges.size() == 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos + 1, tile->colPos);
			tile->mEdges.push_back(e);
		}
		//south edge
		if (tile->rowPos - 1 >= 0 && GetTile(tile->rowPos - 1, tile->colPos)->mEdges.size() == 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos - 1, tile->colPos);
			tile->mEdges.push_back(e);
		}
		//east edge
		if (tile->colPos + 1 < GRID_COLS && GetTile(tile->rowPos, tile->colPos + 1)->mEdges.size() == 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos, tile->colPos + 1);
			tile->mEdges.push_back(e);
		}
		//west edge
		if (tile->colPos - 1 >= 0 && GetTile(tile->rowPos, tile->colPos - 1)->mEdges.size() == 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos, tile->colPos - 1);
			tile->mEdges.push_back(e);
		}
	}
}

void LoadGridEdgesDiagonal()
{
	LoadGridEdges();

	for (auto tile : grid)
	{
		//north-east
		if (tile->rowPos + 1 < GRID_ROWS && tile->colPos + 1 < GRID_COLS)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos + 1, tile->colPos + 1);
			tile->mEdges.push_back(e);
		}
		//north-west
		if (tile->rowPos + 1 < GRID_ROWS && tile->colPos - 1 >= 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos + 1, tile->colPos - 1);
			tile->mEdges.push_back(e);
		}

		//south-east
		if (tile->rowPos - 1 >= 0 && tile->colPos + 1 < GRID_COLS)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos - 1, tile->colPos + 1);
			tile->mEdges.push_back(e);
		}
		//south-west
		if (tile->rowPos - 1 >= 0 && tile->colPos - 1 >= 0)
		{
			Edge* e = new Edge();
			e->mStart = tile;
			e->mEnd = GetTile(tile->rowPos - 1, tile->colPos - 1);
			tile->mEdges.push_back(e);
		}
	}
}

void Destroy()
{
	for (auto t : grid)
	{
		for (auto edge : t->mEdges)
		{
			delete edge;
		}
		t->mEdges.clear();
		delete t;
	}
	grid.clear();
	delete seekBehaviour;
	delete fleeBehaviour;
}

void HandleUI()
{
	if (frk.IsKeyDown(ESC))
	{
		quit = true;
	}
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
}

void AutoRun()
{

	if (tank.mGoalNode == nullptr)
	{
		ResetTiles();
		GetNearestTile(tank.mPosition.x, tank.mPosition.y)->mColor = GREEN;
		
		Tile* t = nullptr;
		while (t == nullptr)
		{
			t = GetRandomTile();
			if (t->mColor == BROWN) t = nullptr;
		}
		t->mColor = RED;
		mGoalNode = t;
		AStarPathFind(true);
		//ThetaStarPathFind();
	}
}

void ResetTiles()
{
	for (auto tile : grid)
	{
		tile->mIsVisited = false;
		tile->mGScore = INT_MAX;
		tile->mFScore = 0;
		tile->mPathParentNode = nullptr;
		if (tile->mColor != BROWN) tile->mColor = WHITE;
	}
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

void ThetaStarPathFind()
{
	std::list<Tile*> priorityQ;
	Tile* startTile = GetNearestTile(tank.mPosition.x, tank.mPosition.y);
	priorityQ.push_front(startTile);
	startTile->mGScore = 0;
	startTile->mPathParentNode = startTile;

	while (!priorityQ.empty())
	{
		priorityQ.sort(SortOnFScore);
		Tile* current = priorityQ.front();
		priorityQ.pop_front();

		current->mIsVisited = true;
		if (current != startTile && current != mGoalNode && current->mIsWalkable)
		{
			current->mColor = glm::vec4(1, 1, 0, 1);
		}

		if (current == mGoalNode)
			break;
		for (auto edge : current->mEdges)
		{
			Tile* neighbor = edge->mEnd;
			if (!neighbor->mIsVisited && neighbor->mIsWalkable)
			{
				if (HasStraightLine(current->mPathParentNode, neighbor))
				{
					float fScore = current->mPathParentNode->mGScore + glm::distance(current->mPathParentNode->mPosition, neighbor->mPosition) + GetHeuristic(DISTANCE, neighbor, mGoalNode);
					if (fScore < neighbor->mGScore)
					{
						neighbor->mPathParentNode = current->mPathParentNode;
						neighbor->mGScore = current->mGScore + neighbor->mWeight;
						neighbor->mFScore = fScore;
					}
				}
				else
				{
					float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(DISTANCE, neighbor, mGoalNode);
					//float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(MANHATTAN, neighbor, mGoalNode);
					if (fScore < neighbor->mGScore)
					{
						neighbor->mPathParentNode = current;
						neighbor->mGScore = current->mGScore + neighbor->mWeight;
						neighbor->mFScore = fScore;
					}
				}
				if (std::find(priorityQ.begin(), priorityQ.end(), neighbor) == priorityQ.end() && neighbor->mPathParentNode != nullptr)
				{
					priorityQ.push_back(neighbor);
				}
			}
		}

	}
	if (mGoalNode->mPathParentNode == nullptr)
	{
		//no solution
		return;
	}

	tank.pathList.push_back(mGoalNode);
	Tile* parent = mGoalNode->mPathParentNode;
	tank.pathList.insert(tank.pathList.begin(), parent);
	while (parent != startTile)
	{
		parent = parent->mPathParentNode;
		tank.pathList.insert(tank.pathList.begin(), parent);
	}
}

void AStarPathFind(bool smoothPath)
{
	std::list<Tile*> priorityQ;
	Tile* startTile = GetNearestTile(tank.mPosition.x, tank.mPosition.y);
	priorityQ.push_front(startTile);
	startTile->mGScore = 0;
	startTile->mPathParentNode = startTile;

	while (!priorityQ.empty())
	{
		priorityQ.sort(SortOnFScore);
		Tile* current = priorityQ.front();
		priorityQ.pop_front();

		current->mIsVisited = true;
		if (current != startTile && current != mGoalNode && current->mIsWalkable)
		{
			current->mColor = glm::vec4(1, 1, 0, 1);
		}

		if (current == mGoalNode)
			break;

		for (auto edge : current->mEdges)
		{
			Tile* neighbor = edge->mEnd;
			if (!neighbor->mIsVisited && neighbor->mIsWalkable)
			{
				float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(DISTANCE, neighbor, mGoalNode);
				//float fScore = current->mGScore + neighbor->mWeight + GetHeuristic(MANHATTAN, neighbor, mGoalNode);
				if (fScore < neighbor->mGScore)
				{
					neighbor->mPathParentNode = current;
					neighbor->mGScore = current->mGScore + neighbor->mWeight;
					neighbor->mFScore = fScore;
					if (std::find(priorityQ.begin(), priorityQ.end(), neighbor) == priorityQ.end())
					{
						priorityQ.push_back(neighbor);
					}
				}
			}
		}

	}
	if (mGoalNode->mPathParentNode == nullptr)
	{
		//no solution
		return;
	}

	tank.pathList.push_back(mGoalNode);
	Tile* parent = mGoalNode->mPathParentNode;
	tank.pathList.insert(tank.pathList.begin(), parent);
	while (parent != startTile)
	{
		parent = parent->mPathParentNode;
		tank.pathList.insert(tank.pathList.begin(), parent);
	}

	if (smoothPath)
	{

		if (tank.pathList.size() < 3)
			return;
		Tile* start = *tank.pathList.begin();
		Tile* end = *(tank.pathList.begin() + 2);

		//std::find(tank.pathList.begin(), tank.pathList.end(), start)
		//std::find(tank.pathList.begin(), tank.pathList.end(), end)
		while (std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1 != tank.pathList.end())
		{
			if (HasStraightLine(start, end))
			{
				//remove node after start
				tank.pathList.erase(std::find(tank.pathList.begin(), tank.pathList.end(), start) + 1);
				if (std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1 != tank.pathList.end())
				{
					end = *(std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1);
				}				
			}
			else
			{
				start = *(std::find(tank.pathList.begin(), tank.pathList.end(), start) + 1);
				if (std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1 != tank.pathList.end())
				{
					end = *(std::find(tank.pathList.begin(), tank.pathList.end(), end) + 1);
				}
			}
		}
	}
}

std::vector<Tile*> GetTilesInLine(Ray& ray, Tile* end)
{
	std::vector<Tile*> result;
	vec2 currentPosition = ray.origin;
	Tile* currentTile = nullptr;

	while (currentTile != end)
	{
		currentPosition += end->mSize * ray.direction;
		currentTile = GetNearestTile(currentPosition.x, currentPosition.y);
		if (std::find(result.begin(), result.end(), currentTile) == result.end())
		{
			result.push_back(currentTile);
		}
		
	}
	return result;
}

bool HasStraightLine(Tile* start, Tile* goal)
{
	Ray ray(start->mPosition, GetRayDirection(start->mPosition, goal->mPosition));
	//need to check every object for collision
	std::vector<Tile*> nodeList = GetTilesInLine(ray, goal);
	for (Tile* tile : nodeList)
	{
		//only need to check non walkable objects
		if (!tile->mIsWalkable)
		{
			AABB box = GetAABB(tile);
			float enter = 0.0f;
			float exit = 0.0f;
			if (RayAABBIntersect(ray, box, enter, exit))
			{
				//if collision true, no straight line
				return false;
			}
		}
	}
	//no collisions found
	return true;
}

AABB GetAABB(Tile* tile)
{
	float hHeight = tile->mSize.y * .5;
	float hWidth = tile->mSize.x * .5;
	return AABB(vec2(tile->mPosition.x - hWidth, tile->mPosition.y - hHeight), vec2(tile->mPosition.x + hWidth, tile->mPosition.y + hHeight));
}

bool RayPlaneIntersect(Ray& ray, Plane& plane, float& t)
{
	float denom = glm::dot(plane.normal, ray.direction);
	if (denom > 1e-6)
	{
		vec2 point_origin = plane.point - ray.origin;
		t = glm::dot(point_origin, plane.normal) / denom;
		return (t >= 0);
	}
	return false;

}

vec2 GetRayDirection(const vec2& pointA, const vec2& pointB)
{
	return glm::normalize(pointB - pointA);
}


bool RayAABBIntersect(Ray& ray, AABB& box, float& enter, float& exit)
{
	vec2 min = (box.minPoint - ray.origin) / ray.direction;
	vec2 max = (box.maxPoint - ray.origin) / ray.direction;

	vec2 near = glm::min(min, max);
	vec2 far = glm::max(min, max);

	enter = glm::max(glm::max(near.x, near.y), 0.0f);
	exit = glm::min(far.x, far.y);

	return (exit > 0.0f && enter < exit);
}