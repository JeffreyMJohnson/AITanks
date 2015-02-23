#include "framework/Animation.h"

Animation::Animation()
{
	mCurrentFrame = 0;
	mDirection = 1.0f;
	//mAnimationState = "walk";
	mAnimationState = "idle";
}

void Animation::Initialize(const float width, const float height, const char* spriteSheetDataFile)
{
	LoadData(spriteSheetDataFile);
	mSprite.Initialize(width, height, (mFilePath + mSpriteSheetFile).c_str(), true);
}

void Animation::LoadData(const char* spriteSheetDataFile)
{
	mFilePath = GetPath(spriteSheetDataFile);

	using namespace pugi;
	xml_document doc;

	std::string extension = std::string(spriteSheetDataFile);
	int dotPos = extension.find_last_of(".");
	extension = extension.substr(dotPos + 1, std::string::npos);

	xml_parse_result result = doc.load_file(spriteSheetDataFile);

	if (extension == "xml")
	{

		mSpriteSheetFile = doc.child("atlas").child("common").attribute("sheet").value();

		for (xml_node frames : doc.child("atlas").children("frames"))
		{
			std::vector <Frame> framesList;
			for (xml_node frame : frames.children())
			{
				Frame f;
				f.number = std::atoi(frame.attribute("id").value());
				f.UV.x = std::atof(frame.attribute("minX").value());
				f.UV.y = std::atof(frame.attribute("minY").value());
				f.UV.z = std::atof(frame.attribute("maxX").value());
				f.UV.w = std::atof(frame.attribute("maxY").value());
				framesList.push_back(f);
			}
			mFrameData[frames.attribute("name").value()] = framesList;
		}
	}
	if (extension == "shoe")
	{
		mSpriteSheetFile = doc.child("TextureAtlas").attribute("imagePath").value();
		const float sheetWidth = std::stof(doc.child("TextureAtlas").attribute("sheetWidth").value());
		const float sheetHeight = std::stof(doc.child("TextureAtlas").attribute("sheetHeight").value());


		for (xml_node frames : doc.child("TextureAtlas").children("frames"))
		{
			int id = 0;
			std::vector <Frame> framesList;
			for (xml_node frame : frames.children())
			{
				Frame f;
				f.number = id;
				//need to compute UV and the normalize
				f.UV.x = std::atof(frame.attribute("x").value()) / sheetWidth;
				f.UV.y = std::atof(frame.attribute("y").value()) / sheetHeight;
				f.UV.z = (f.UV.x + std::atof(frame.attribute("width").value())) / sheetWidth;
				f.UV.w = (f.UV.y + std::atof(frame.attribute("height").value())) / sheetHeight;
				framesList.push_back(f);
				id++;
			}
			mFrameData[frames.attribute("name").value()] = framesList;

		}

	}
}

void Animation::Update(double deltaTime)
{
	const float speed = .1;
	mElapsedTime += deltaTime;

	if (mElapsedTime > mFrameDuration)
	{
		mElapsedTime = 0;
		//check if last frame
		if (mCurrentFrame == mFrameData[mAnimationState].size() - 1)
		{
			mCurrentFrame = 0;
		}
		else
		{
			mCurrentFrame++;
		}
	}
	mSprite.SetSpriteUV(mFrameData[mAnimationState][mCurrentFrame].UV.x, mFrameData[mAnimationState][mCurrentFrame].UV.y, mFrameData[mAnimationState][mCurrentFrame].UV.z, mFrameData[mAnimationState][mCurrentFrame].UV.w);
}

void Animation::SwitchDirection()
{
	mDirection *= -1;
	mSprite.scale.x *= -1;
	mSprite.UpdateTransform();
}

void Animation::Draw()
{
	mSprite.Draw();
}

std::string Animation::GetPath(const char* spriteSheetDataFile)
{
	std::string s(spriteSheetDataFile);
	unsigned int pos = s.find_last_of("\\");
	if (pos == std::string::npos)
		return "";
	return s.substr(0, pos + 1);
}