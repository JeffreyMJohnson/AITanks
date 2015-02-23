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