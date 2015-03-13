#include "Tile.h"

Tile::Tile(int a_rowPos, int a_colPos)
{
	mIsVisited = false;
	mGScore = INT_MAX;
	mFScore = 0;
	mWeight = 1;
	mPathParentNode = nullptr;
	rowPos = a_rowPos;
	colPos = a_colPos;
	mColor = glm::vec4(1, 1, 1, 1);
	mIsWalkable = true;
}