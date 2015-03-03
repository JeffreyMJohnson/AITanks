#ifndef _FONT_H_
#define _FONT_H_
#include "glm\glm.hpp"
#include "pugixml\pugixml.hpp"
#include "SOIL\SOIL.h"
#include <map>
#include <iostream>
#include <string>

#include "FontChar.h"

class Font
{
public:
	unsigned int lineHeight;
	std::string name;
	std::string imageFileName;
	std::string path;
	unsigned int charCount;
	unsigned int spriteSheetWidth, spriteSheetHeight;
	unsigned int fontSpriteSheetTextureID;
	std::map<unsigned int, FontChar> Chars;

	Font();

	bool Init(const char* fontFilePath, const char* fontFileName);

private:
	bool LoadFont(const char* fontFileName);

	glm::vec4 CalculateUV(FontChar& ch);

	unsigned int loadTexture(const char* a_pFilename);
};

#endif
