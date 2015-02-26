#include "Globals.h"
#include "framework/Framework.h"
#include "Tile.h"
#include "Tank.h"
#include "Globals.h"

#include <time.h>
#include <iostream>
#include <algorithm>

enum HEURISTIC_TYPE
{
	DISTANCE,
	MANHATTAN,
	DIAGONAL
};

void CreateGrid();
void LoadGridEdges();
void LoadGridEdgesDiagonal();
void LoadGridEdgesOneWay();
void Destroy();
Tile* GetTile(glm::vec2 position);
Tile* GetTile(int a_row, int a_col);
void UpdateTiles();
Tile* GetRandomTile();
glm::vec2 GetRandomTilePosition();
void HandleUI();
Tile* GetNearestTile(float xPos, float yPos);
bool SortOnFScore(Tile* lhs, Tile* rhs);
void AStarPathFind(); 
void ResetTiles();
void AutoRun();
float GetHeuristic(HEURISTIC_TYPE type, Tile* node, Tile* nodeTarget);


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

Tank tank(glm::vec2(20,20), glm::vec2(200,75));

Tile* mGoalNode = nullptr;



int main()
{
	srand(time(NULL));

	
	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Tanks Path Find Demo");
	frk.SetBackgroundColor(1, 1, 1, 1);

	CreateGrid();
	tank.mSpriteID = frk.CreateSprite(tank.mSize.x, tank.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank.mSpriteID, .008, .016, .121, .109);

	Tile* t = GetTile(GetRandomTilePosition());
	tank.mPosition = t->mPosition;
	tank.mLastNodeVisited = t;
	frk.MoveSprite(tank.mSpriteID, tank.mPosition.x, tank.mPosition.y);

	do
	{
		frk.ClearScreen();
		UpdateTiles();
		tank.Update(frk.GetDeltaTime());
		frk.MoveSprite(tank.mSpriteID, tank.mPosition.x, tank.mPosition.y);
		frk.DrawSprite(tank.mSpriteID, tank.mColor);

		HandleUI();
		AutoRun();

	} while (frk.UpdateFramework() && !quit);



	frk.Shutdown();
	Destroy();

	return 0;
}

Tile* GetNearestTile(float xPos, float yPos)
{
	Tile* result = nullptr;
	float dx = INT_MAX;
	for (auto tile : grid)
	{
		if (glm::distance(tile->mPosition, glm::vec2(xPos, yPos)) < dx)
		{
			result = tile;
			dx = glm::distance(tile->mPosition, glm::vec2(xPos, yPos));
		}
	}
	return result;
}

Tile* GetRandomTile()
{
	return grid[rand() % grid.size()];
}

glm::vec2 GetRandomTilePosition()
{
	return GetRandomTile()->mPosition;
}

void CreateGrid()
{
	const int wallProbability = 30;//int between 0 and 100. greater increases likelyhood of tile being wall
	glm::vec2 tileSize(25, 25);
	mTileSpriteID = frk.CreateSprite(tileSize.x, tileSize.y, ".\\resources\\textures\\Basic.png", true);

	glm::vec2 startPos(200, 75);
	glm::vec2 position = startPos;
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
			if (rand() % 101 <= wallProbability)
			{
				t->mColor = BROWN;
				t->mWeight = INT_MAX;
				t->mIsWalkable = false;
			}

		}
		position.y += tileSize.y;
	}
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

Tile* GetTile(glm::vec2 position)
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
		AStarPathFind();

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
		AStarPathFind();
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

void AStarPathFind()
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
	tank.pathList.push_front(parent);
	while (parent != startTile)
	{
		parent = parent->mPathParentNode;
		tank.pathList.push_front(parent);
	}
}