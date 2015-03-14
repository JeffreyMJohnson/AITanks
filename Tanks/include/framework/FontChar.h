#ifndef _FONT_CHAR_H_
#define _FONT_CHAR_H_
#define GLM_FORCE_PURE
#include "glm\glm.hpp"


class FontChar
{
public:
	glm::vec4 position;
	unsigned int ID;
	glm::vec2 fontSheetLocation;
	unsigned int width, height;
	float xOffset, yOffset;
	float xAdvance;
	//left, top, right, bottom
	glm::vec4 UV;
	
	FontChar();

private:

};

#endif