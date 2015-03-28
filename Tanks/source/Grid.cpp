#include "Grid.h"

Grid::Grid(Framework* framework)
{
	mFramework = framework;
	srand(time(nullptr));
}

void Grid::Initialize()
{
	CreateGrid();
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
		mFramework->MoveSprite(tile->mSpriteID, tile->mPosition.x, tile->mPosition.y);
		mFramework->DrawSprite(tile->mSpriteID, tile->mColor);
	}
}

void Grid::CreateGrid()
{

	glm::vec2 tileSize(TILE_WIDTH, TILE_HEIGHT);

	unsigned int spriteId = mFramework->CreateSprite(tileSize.x, tileSize.y, TILE_TEXTURE_PATH, true);

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
			if ((rand() % 100) + 1 <= WALL_PROBABILITY)
			{
				t->mColor = MNF::Color::Brown();
				t->mWeight = INT_MAX;
				t->mIsWalkable = false;
			}
			//resource tiles
			if (row == 24 && col == 24 ||
				row == 0 && col == 24)
			{
				t->mColor = MNF::Color::Green();
				mResourceTilesList.push_back(t);
			}

			//base tiles
			if (row == 0 && col == 0)
			{
				t->mColor = MNF::Color::Red();
				mBaseTilesList.push_back(t);
			}

		}
		position.y += tileSize.y;
	}
	Tile* t = mTileList.back();
	gridRect.z = t->mPosition.x + (tileSize.x * .5f);
	gridRect.w = t->mPosition.y + (tileSize.y * .5f);
	LoadGridEdges();
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
	for (auto tile : mTileList)
	{
		tile->mIsVisited = false;
		tile->mGScore = INT_MAX;
		tile->mFScore = 0;
		tile->mPathParentNode = nullptr;
		if (tile->mColor != glm::vec4(0.501, 0.152, 0.039, 1.0f)) tile->mColor = glm::vec4(1, 1, 1, 1);
	}
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

const std::vector<Tile*>& Grid::GetResourceTilesList()
{
	return mResourceTilesList;
}

const std::vector<Tile*>& Grid::GetBaseTilesList()
{
	return mBaseTilesList;
}

std::vector<Tile*> Grid::GetTilesInLine(MNF::Collider::Ray& ray, Tile* end)
{
	std::vector<Tile*> result;
	glm::vec2 currentPosition = ray.origin;
	Tile* currentTile = nullptr;

	while (currentTile != end)
	{
		currentPosition += end->mSize * ray.direction;
		currentTile = GetNearestTile(currentPosition);
		if (std::find(result.begin(), result.end(), currentTile) == result.end())
		{
			result.push_back(currentTile);
		}
		
	}
	return result;
}

bool Grid::HasStraightLine(Tile* start, Tile* goal)
{
	MNF::Collider::Ray ray(start->mPosition, GetRayDirection(start->mPosition, goal->mPosition));
	//need to check every object for collision
	std::vector<Tile*> nodeList = GetTilesInLine(ray, goal);
	for (Tile* tile : nodeList)
	{
		//only need to check non walkable objects
		if (!tile->mIsWalkable)
		{
			MNF::Collider::AABB box = GetAABB(tile);
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

MNF::Collider::AABB Grid::GetAABB(Tile* tile)
{
	float hHeight = tile->mSize.y * .5f;
	float hWidth = tile->mSize.x * .5f;
	return MNF::Collider::AABB(glm::vec2(tile->mPosition.x - hWidth, tile->mPosition.y - hHeight), glm::vec2(tile->mPosition.x + hWidth, tile->mPosition.y + hHeight));
}

glm::vec2 Grid::GetRayDirection(const glm::vec2& pointA, const glm::vec2& pointB)
{
	return glm::normalize(pointB - pointA);
}