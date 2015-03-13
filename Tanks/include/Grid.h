#ifndef _GRID_H_
#define _GRID_H_
#include "glm\glm.hpp"
#include "framework\Framework.h"
#include "Tile.h"
#include "Edge.h"

#include <vector>
#include <limits.h>
#include <time.h>

class Grid
{
public:
	static const int GRID_ROWS;
	static const int GRID_COLS;
	static const glm::vec2 startPosition;
	static glm::vec4 gridRect;
	

	Grid(Framework* framework);
	~Grid();

	void Update();
	void Draw();

	static Tile* GetTile(glm::vec2 position);
	static Tile* GetTile(int a_row, int a_col);
	static Tile* GetRandomTile();
	static Tile* GetNearestTile(glm::vec2 screenPosition);
	static void ResetTiles();
	static bool IsOutOfBounds(glm::vec2 position, glm::vec2 size);
private:
	static const int wallProbability = 0;//int between 0 and 100. greater increases likelyhood of tile being wall
	Framework* mFramework;
	static std::vector<Tile*> mTileList;

	void CreateGrid();
	void LoadGridEdges();

};
const int GRID_ROWS = 25;
const int GRID_COLS = 25;
const glm::vec2 startPosition = glm::vec2(200, 75);
glm::vec4 gridRect = glm::vec4(0, 0, 0, 0);
#endif