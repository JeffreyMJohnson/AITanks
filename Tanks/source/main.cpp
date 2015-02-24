#include "Globals.h"
#include "framework/Framework.h"
#include "Tile.h"
#include "Tank.h"

#include <time.h>
#include <iostream>


void CreateGrid();
void LoadGridEdges();
void Destroy();
Tile* GetTile(glm::vec2 position);
Tile* GetTile(int a_row, int a_col);
void UpdateTiles();
glm::vec2 GetRandomTilePosition();
void HandleUI();
Tile* GetNearestTile(float xPos, float yPos);


const int GRID_ROWS = 25;
const int GRID_COLS = 25;

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

glm::vec2 GetRandomTilePosition()
{
	Tile* tile = grid[rand() % grid.size()];
	return tile->mPosition;
}

void CreateGrid()
{
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
		}
		position.y += tileSize.y;
	}
	LoadGridEdges();
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
		//check if current node is old goal node and change color
		GetNearestTile(tank.mPosition.x, tank.mPosition.y)->mColor = glm::vec4(1, 1, 1, 1);
		//std::cout << "left clicked: \nxPos: " << xPos << "\nyPos: " << yPos << std::endl;
		Tile* t = GetNearestTile(xPos, yPos);
		//std::cout << "Nearest tile pos (" << t->mPosition.x << ", " << t->mPosition.y << ")\n";
		t->mColor = glm::vec4(1, 0, 0, 1);
		mGoalNode = t;
		tank.mGoalNode = t;
	}
}