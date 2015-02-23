#ifndef _EDGE_H_
#define _EDGE_H_

#include "Tile.h"

//circular reference
class Tile;

class Edge
{
public:
	Tile* mStart;
	Tile* mEnd;
	int mWeight;

	Edge()
	{
		mStart = nullptr;
		mEnd = nullptr;
		mWeight = 1;
	}

};

#endif