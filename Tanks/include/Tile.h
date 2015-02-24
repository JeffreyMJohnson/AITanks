#ifndef _TILE_H_
#define _TILE_H_
#include <vector>
#include <climits>
#include "Edge.h"


class Tile
{
public:
	std::vector<Edge*> mEdges;
	bool mIsVisited;
	int mGScore;
	int mFScore;
	Tile* mPathParentNode;
	int rowPos, colPos;
	glm::vec2 mSize;
	glm::vec2 mPosition;
	glm::vec4 mColor;


	Tile(int a_rowPos, int a_colPos)
	{
		mIsVisited = false;
		mGScore = INT_MAX;
		mFScore = 0;
		mPathParentNode = nullptr;
		rowPos = a_rowPos;
		colPos = a_colPos;
		mColor = glm::vec4(1, 1, 1, 1);
	}
};

#endif