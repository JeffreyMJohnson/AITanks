#include "Globals.h"
#include "framework/Framework.h"
#include "Tile.h"
#include "Tank.h"

#include <time.h>


void CreateGrid();
void LoadGridEdges();
void Destroy();
Tile* GetTile(int a_row, int a_col);
void UpdateTiles();
glm::vec2 GetRandomTilePosition();
void HandleUI();


const int GRID_ROWS = 25;
const int GRID_COLS = 25;

Framework frk;
bool quit = false;
std::vector<Tile*> grid;
unsigned int mTileSpriteID;

Tank tank(glm::vec2(20,20), glm::vec2(200,75));

int main()
{
	srand(time(NULL));
	
	frk.Initialize(MNF::Globals::SCREEN_WIDTH, MNF::Globals::SCREEN_HEIGHT, "Tanks Path Find Demo");
	frk.SetBackgroundColor(1, 1, 1, 1);

	CreateGrid();
	tank.mSpriteID = frk.CreateSprite(tank.mSize.x, tank.mSize.y, ".\\resources\\textures\\tank.png", true);
	frk.SetSpriteUV(tank.mSpriteID, .008, .016, .121, .109);

	tank.mPosition = GetRandomTilePosition();
	frk.MoveSprite(tank.mSpriteID, tank.mPosition.x, tank.mPosition.y);


	do
	{
		frk.ClearScreen();
		UpdateTiles();

		frk.DrawSprite(tank.mSpriteID);

		HandleUI();
	} while (frk.UpdateFramework() && !quit);



	frk.Shutdown();
	Destroy();

	return 0;
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
		frk.DrawSprite(mTileSpriteID);
	}
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
}