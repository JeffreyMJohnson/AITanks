#ifndef _TANK_H_
#define _TANK_H_

#include <vector>
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


	Tank(glm::vec2 a_size, glm::vec2 a_position)
	{
		mSpriteID = 0;
		mPosition = a_position;
			mSize = a_size;
			mColor = glm::vec4(1, 1, 1, 1);
	}

	void Update(float deltaTime)
	{
		if (mGoalNode != nullptr)
		{
			if (mCurrentLERPValue < 1)
			{
				mPosition = glm::lerp(mLastNodeVisited->mPosition, mGoalNode->mPosition, mCurrentLERPValue);
				mCurrentLERPValue += deltaTime;
			}
			else
			{
				mCurrentLERPValue = 0;
				mLastNodeVisited = mGoalNode;
				mGoalNode = nullptr;
			}
		}
	}

};

#endif