#ifndef _TANK_H_
#define _TANK_H_

class Tank
{
public:
	unsigned int mSpriteID;
	glm::vec2 mPosition;
	glm::vec2 mSize;

	Tank(glm::vec2 a_size, glm::vec2 a_position)
	{
		mSpriteID = 0;
		mPosition = a_position;
			mSize = a_size;
	}

	void Update(float deltaTime)
	{

	}

};

#endif