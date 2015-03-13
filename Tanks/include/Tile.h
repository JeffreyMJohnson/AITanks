#ifndef _TILE_H_
#define _TILE_H_
#include <vector>
#include <climits>
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
	static const float HEIGHT;
	static const float WIDTH;
	static const char* TEXTURE_PATH;

	Tile(int a_rowPos, int a_colPos);

private:


};

const float Tile::HEIGHT = 25.0f;
const float Tile::WIDTH = 25.0f;
const char* Tile::TEXTURE_PATH = ".\\resources\\textures\\Basic.png";

#endif