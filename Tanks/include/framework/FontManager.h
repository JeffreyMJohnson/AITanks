#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include "Font.h"
#include "Sprite.h"
#include "glm\glm.hpp"

#include <string>


class FontManager
{
public:
	Font mFont;
	Sprite mCharSprite;

	float scale = 1;

	FontManager();	

	void DrawText(const char* text, float startPositionX, float startPositionY);


	unsigned int Initialize(const char* fontPath, const char* descriptionFile);

	void LoadFont(const char* fontPath, const char* descriptionFile);
};

#endif