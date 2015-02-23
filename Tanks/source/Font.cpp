#include "framework/Font.h"

Font::Font()
{
	lineHeight = 0;
	name = "";
	charCount = 0;
}

bool Font::Init(const char* fontFilePath, const char* fontFileName)
{
	//imageFileName = fontImagFile;
	//fontSpriteSheetTextureID = loadTexture(fontImagFile);

	path = std::string(fontFilePath);

	if (!LoadFont(fontFileName))
	{
		return false;
	}
	return true;

}

bool Font::LoadFont(const char* fontFileName)
{
	std::string pathName(path);
	pathName += fontFileName;
	using namespace pugi;
	xml_document doc;
	//xml_parse_result result = doc.load_file(".\\resources\\fonts\\arial.fnt");
	xml_parse_result result = doc.load_file(pathName.c_str());
	if (!result)
	{
		std::cout << "Error loading font file, verify path (do the slashes need escaping?)\n";
		std::cout << result.description() << "\n";
		return false;
	}

	xml_node common = doc.child("font").child("common");

	//load the charset common attributes
	lineHeight = std::atoi(common.attribute("lineHeight").value());

	//base = std::atoi(common.attribute("base").value());
	spriteSheetWidth = std::atoi(common.attribute("scaleW").value());
	spriteSheetHeight = std::atoi(common.attribute("scaleH").value());

	name = doc.child("font").child("info").attribute("face").value();
	//imageFileName = doc.child("font").child("pages").child("page").attribute("file").value();

	//load each char
	for (xml_node Char : doc.child("font").child("chars").children("char"))
	{
		FontChar ch;
		ch.ID = std::atoi(Char.attribute("id").value());
		ch.fontSheetLocation = glm::vec2(std::atoi(Char.attribute("x").value()), std::atoi(Char.attribute("y").value()));
		ch.width = std::atoi(Char.attribute("width").value());
		ch.height = std::atoi(Char.attribute("height").value());
		ch.xOffset = std::atof(Char.attribute("xoffset").value());
		ch.yOffset = std::atof(Char.attribute("yoffset").value());
		ch.xAdvance = std::atof(Char.attribute("xadvance").value());

		ch.UV = CalculateUV(ch);
		Chars[ch.ID] = ch;
	}

	//get filename
	imageFileName = doc.child("font").child("pages").first_child().attribute("file").value();
	return true;
}

glm::vec4 Font::CalculateUV(FontChar& ch)
{
	float minX = ch.fontSheetLocation.x;
	float minY = ch.fontSheetLocation.y;
	float maxX = minX + ch.width;
	float maxY = minY + ch.height;

	glm::vec4 result(
		minX / spriteSheetWidth,
		minY / spriteSheetHeight,
		maxX / spriteSheetWidth,
		maxY / spriteSheetHeight);

	return result;
}

unsigned int Font::loadTexture(const char* a_pFilename)
{
	unsigned int uiTextureID = 0;

	uiTextureID = SOIL_load_OGL_texture(a_pFilename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);

	//check for errors
	if (uiTextureID == 0)
	{
		std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
	}
	return uiTextureID;
	//}
}