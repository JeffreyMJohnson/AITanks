#ifndef _GRID_H_
#define _GRID_H_
#include "Globals.h"
#include "framework\Framework.h"
#include "Tile.h"
#include "Edge.h"

#include <vector>
#include <limits.h>
#include <time.h>

class Grid
{
public:
	//MAGIC NUMBERS
	const int GRID_ROWS = 25;
	const int GRID_COLS = 25;
	const float TILE_HEIGHT = 25.0f;
	const float TILE_WIDTH = 25.0f;
	const char* TILE_TEXTURE_PATH = ".\\resources\\textures\\Basic.png";
	const glm::vec2 startPosition = glm::vec2(200, 75);
	glm::vec4 gridRect = glm::vec4(0, 0, 0, 0);
	

	Grid(Framework* framework);
	void Initialize();
	~Grid();

	void Update();
	void Draw();

	Tile* GetTile(glm::vec2 position);
	Tile* GetTile(int a_row, int a_col);
	Tile* GetRandomTile();
	Tile* GetNearestTile(glm::vec2 screenPosition);
	void ResetTiles();
	bool IsOutOfBounds(glm::vec2 position, glm::vec2 size);

	const std::vector<Tile*>& GetResourceTilesList();

	//pathfinding
	bool HasStraightLine(Tile* start, Tile* goal);
	glm::vec2 GetRayDirection(const glm::vec2& pointA, const glm::vec2& pointB);
	std::vector<Tile*> GetTilesInLine(MNF::Collider::Ray& ray, Tile* end);
	MNF::Collider::AABB GetAABB(Tile* tile);
private:
	const int wallProbability = 0;//int between 0 and 100. greater increases likelyhood of tile being wall
	Framework* mFramework;
	std::vector<Tile*> mTileList;
	std::vector<Tile*> mResourceTilesList;

	void CreateGrid();
	void LoadGridEdges();


};
#endif