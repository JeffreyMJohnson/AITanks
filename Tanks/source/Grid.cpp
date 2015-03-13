#include "Grid.h"

Grid::Grid(Framework* framework)
{
	mFramework = framework;
	srand(time(nullptr));
}

Grid::~Grid()
{
	for (auto t : mTileList)
	{
		for (auto edge : t->mEdges)
		{
			delete edge;
		}
		t->mEdges.clear();
		delete t;
	}
	mTileList.clear();
}

void Grid::Update()
{

}

void Grid::Draw()
{
	for (auto tile : mTileList)
	{
		mFramework->DrawSprite(tile->mSpriteID, tile->mColor);
	}
}

void Grid::CreateGrid()
{

	glm::vec2 tileSize(Tile::WIDTH, Tile::HEIGHT);

	unsigned int spriteId = mFramework->CreateSprite(tileSize.x, tileSize.y, Tile::TEXTURE_PATH, true);

	//vec2 startPos(200, 75);
	Grid::gridRect.x = Grid::startPosition.x - (tileSize.x * .5f);
	Grid::gridRect.y = Grid::startPosition.y - (tileSize.y * .5f);
	glm::vec2 position = Grid::startPosition;
	for (int row = 0; row < Grid::GRID_ROWS; row++)
	{
		for (int col = 0; col < Grid::GRID_COLS; col++)
		{
			Tile* t = new Tile(row, col);
			t->mSpriteID = spriteId;
			t->mSize = tileSize;
			t->mPosition = position;
			mTileList.push_back(t);
			if (col == GRID_COLS - 1)
			{
				position.x = Grid::startPosition.x;
			}
			else
			{
				position.x += tileSize.x;
			}
			//see if wall
			if ((rand() % 100) + 1 <= wallProbability)
			{
				t->mColor = glm::vec4(0.501, 0.152, 0.039, 1.0f);
				t->mWeight = INT_MAX;
				t->mIsWalkable = false;
			}

		}
		position.y += tileSize.y;
	}
	Tile* t = mTileList.back();
	gridRect.z = t->mPosition.x + (tileSize.x * .5f);
	gridRect.w = t->mPosition.y + (tileSize.y * .5f);
	//LoadGridEdges();
}

void Grid::LoadGridEdges()
{
	for (auto tile : mTileList)
	{
		//north edge
		if (tile->rowPos + 1 < Grid::GRID_ROWS)
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

Tile* Grid::GetTile(glm::vec2 position)
{
	for (auto tile : mTileList)
	{
		if (tile->mPosition == position)
			return tile;
	}
	return nullptr;
}

Tile* Grid::GetTile(int a_row, int a_col)
{
	return mTileList[a_row * GRID_ROWS + a_col];

}

Tile* Grid::GetRandomTile()
{
	return mTileList[rand() % mTileList.size()];
}

Tile* Grid::GetNearestTile(glm::vec2 screenPosition)
{
	Tile* result = nullptr;
	float minDx = (float)INT_MAX;
	for (auto tile : mTileList)
	{
		float dx = glm::distance(tile->mPosition, screenPosition);
		if (dx < minDx)
		{
			result = tile;
			minDx = dx;
		}
	}
	return result;
}

void Grid::ResetTiles()
{

}

bool Grid::IsOutOfBounds(glm::vec2 position, glm::vec2 size = glm::vec2(0, 0))
{
	glm::vec2 halfSize = size * .5f;
	//returning the opposite of is-in-bounds logic which uses and(&&) logic. If finds a statement false will quit check
	//as opposed to or(||) logic which must check every statement
	return !(
		position.x - halfSize.x >= gridRect.x &&
		position.x + halfSize.x <= gridRect.z &&
		position.y - halfSize.y >= gridRect.y &&
		position.y + halfSize.y <= gridRect.w
		);
}