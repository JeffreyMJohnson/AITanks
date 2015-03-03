#ifndef _TANK_H_
#define _TANK_H_

#include <vector>
#include <list>
#include "Globals.h"
#include "Tile.h"
#include "glm/gtx/compatibility.hpp"

class Tank
{
public:
	unsigned int mSpriteID;
	glm::vec2 mPosition;
	glm::vec2 mSize;
	glm::vec4 mColor;
	Tile* mGoalNode;
	Tile* mLastNodeVisited;
	std::vector<Tile*> pathList;
	float mCurrentLERPValue = 0;
	float mMoveSpeed;

	Tank();

	Tank(glm::vec2 a_size, glm::vec2 a_position);

	virtual void Update(float deltaTime);

};

#endif