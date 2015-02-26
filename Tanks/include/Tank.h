#ifndef _TANK_H_
#define _TANK_H_

#include <vector>
#include <list>
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
	std::list<Tile*> pathList;
	float mCurrentLERPValue = 0;
	float mVelocity;


	Tank(glm::vec2 a_size, glm::vec2 a_position)
	{
		mSpriteID = 0;
		mPosition = a_position;
		mSize = a_size;
		mColor = glm::vec4(1, 1, 1, 1);
		mVelocity = 5;
	}

	void Update(float deltaTime)
	{
		if (pathList.size() > 0)
		{
			mGoalNode = pathList.front();
			mGoalNode->mColor = glm::vec4(0, 0, 1, 1);
			if (mCurrentLERPValue < 1)
			{
				mPosition = glm::lerp(mLastNodeVisited->mPosition, mGoalNode->mPosition, mCurrentLERPValue);
				mCurrentLERPValue += mVelocity * deltaTime;
			}
			else
			{
				mCurrentLERPValue = 0;
				mLastNodeVisited = mGoalNode;
				pathList.pop_front();
				if (pathList.size() > 0)
					mGoalNode = pathList.front();
			}
		}
		else
		{
			mGoalNode = nullptr;
		}
	}

};

#endif