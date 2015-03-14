#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "pugixml\pugixml.hpp"
#include "Sprite.h"
#define GLM_FORCE_PURE
#include "glm\glm.hpp"

#include <vector>
#include <string>
#include <map>

struct Frame
{
	int number;
	glm::vec4 UV;
};

class Animation
{
public:
	Sprite mSprite;
	std::map<std::string, std::vector<Frame>> mFrameData;
	float mDirection;
	float mCurrentFrame;
	std::string mAnimationState;
	std::string mSpriteSheetFile;
	std::string mFilePath;
	const float mFrameDuration = (1.0f / 15.0f);
	double mElapsedTime = 0;

	Animation();

	void Initialize(const float width, const float height, const char* spriteSheetDataFile);

	void LoadData(const char* spriteSheetDataFile);

	void Update(double deltaTime);

	void SwitchDirection();

	void Draw();

	std::string GetPath(const char* spriteSheetDataFile);

};

#endif