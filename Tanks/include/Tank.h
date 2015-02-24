#ifndef _TANK_H_
#define _TANK_H_

class Tank
{
public:
	unsigned int mSpriteID;
	glm::vec2 mPosition;
	glm::vec2 mSize;
	glm::vec4 mColor;

	Tank(glm::vec2 a_size, glm::vec2 a_position)
	{
		mSpriteID = 0;
		mPosition = a_position;
			mSize = a_size;
			mColor = glm::vec4(1, 1, 1, 1);
	}

	void Update(float deltaTime)
	{

	}

};

#endif