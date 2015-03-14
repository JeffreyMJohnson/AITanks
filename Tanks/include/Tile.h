#ifndef _TILE_H_
#define _TILE_H_
#include <vector>
#include <climits>
#define GLM_FORCE_PURE
#include "glm\glm.hpp"
#include "Edge.h"

class Tile
{
public:
	std::vector<Edge*> mEdges;
	bool mIsVisited;
	int mGScore;
	int mFScore;
	int mWeight;
	Tile* mPathParentNode;
	int rowPos, colPos;
	unsigned int mSpriteID;
	glm::vec2 mSize;
	glm::vec2 mPosition;
	glm::vec4 mColor;
	bool mIsWalkable;

	//MAGIC NUMBERS
	//const float HEIGHT = 25.0f;
	//const float WIDTH = 25.0f;
	//const char* TEXTURE_PATH = ".\\resources\\textures\\Basic.png";

	Tile(int a_rowPos, int a_colPos);

private:


};

#endif