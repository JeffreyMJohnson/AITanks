#include "framework/Framework.h";


void Framework::Initialize(const int screenWidth, const int screenHeight, const char* title)
{
	glfwInit();
	//create the window
	//windowed
	mWindow = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
	//full screen
	//window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Bitches!", glfwGetPrimaryMonitor(), nullptr);

	//make window active
	glfwMakeContextCurrent(mWindow);

	//force GLEW to use a modern OpenGL method for checking if function is available
	glewExperimental = GL_TRUE;
	//initialize GLEW
	glewInit();

	mTotalTime = 0.0;
	mDeltaTime = 0.0;
	mBackgroundColor = glm::vec4(0, 0, 0, 1);

	mFontMan.Initialize(".\\resources\\fonts\\", "arial.fnt");
}


void Framework::Shutdown()
{
	for (int i = 0; i < mSpriteList.size(); i++)
	{
		delete mSpriteList[i];
	}
	mSpriteList.clear();

	for (int i = 0; i < mAnimationList.size(); i++)
	{
		delete mAnimationList[i];
	}
	mAnimationList.clear();
	glfwTerminate();
}


bool Framework::UpdateFramework()
{
	ResetDeltaTime();

	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	return !(glfwWindowShouldClose(mWindow));
}


unsigned int Framework::CreateSprite(const float width, const float height, const char* texture, const bool isCentered)
{
	Sprite* s = new Sprite;
	s->Initialize(width, height, texture, isCentered);
	mSpriteList.push_back(s);
	return mSpriteList.size() - 1;
}

void Framework::MoveSprite(unsigned int spriteID, const float xPosition, const float yPosition)
{
	mSpriteList[spriteID]->SetTranslation(glm::vec3(xPosition, yPosition, 0));
}

void Framework::RotateSprite(unsigned int spriteId, const float angle)
{
	mSpriteList[spriteId]->SetRotation(angle);
}


void Framework::SetSpriteUV(const unsigned int spriteID, const float minX, const float minY, const float maxX, const float maxY)
{
	mSpriteList[spriteID]->SetSpriteUV(minX, minY, maxX, maxY);
}

void Framework::SetSpriteScale(unsigned int spriteId, float scaleX, float scaleY)
{
	mSpriteList[spriteId]->SetScale(scaleX, scaleY);
}

void Framework::DrawSprite(unsigned int spriteID)
{
	mSpriteList[spriteID]->Draw();
}

void Framework::DrawSprite(unsigned int spriteID, glm::vec4& color)
{
	mSpriteList[spriteID]->Draw(color);
}

unsigned int Framework::CreateAnimation(const float width, const float height, const char* spriteSheetDataFile)
{
	Animation* anim = new Animation;
	anim->Initialize(width, height, spriteSheetDataFile);
	mAnimationList.push_back(anim);
	return mAnimationList.size() - 1;
}

void Framework::AnimationFlipDirection(unsigned int animationId)
{
	mAnimationList[animationId]->SwitchDirection();
}

void Framework::SetAnimationState(const unsigned int animationId, const char* a_state)
{
	mAnimationList[animationId]->mAnimationState = a_state;
	if (a_state == "idle")
		mAnimationList[animationId]->mCurrentFrame = 0;
}

void Framework::MoveAnimation(unsigned int animationID, float xPosition, float yPosition)
{
	mAnimationList[animationID]->mSprite.SetTranslation(glm::vec3(xPosition, yPosition, 0));
}

void Framework::DrawAnimation(unsigned int animationID)
{
	mAnimationList[animationID]->Update(GetDeltaTime());
	mAnimationList[animationID]->Draw();
}

void Framework::DrawText(const char* text, const float xPosition, float yPosition)
{
	mFontMan.DrawText(text, xPosition, yPosition);
}


void Framework::SetBackgroundColor(const float r, const float g, const float b, const float a)
{
	//verify input
	if (r < 0 || r > 1 ||
		g < 0 || g > 1 ||
		b < 0 || b > 1 ||
		a < 0 || a > 1)
		return;
	mBackgroundColor = glm::vec4(r, g, b, a);
}

void Framework::ClearScreen()
{
	glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, mBackgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool Framework::IsKeyDown(KEY_CODE a_key)
{
	return glfwGetKey(mWindow, a_key);

}

double Framework::GetDeltaTime()
{
	return mDeltaTime;
}

double Framework::GetTotalTime()
{
	return mTotalTime;
}

/*
PRIVATE FUNCTIONS
*/
void Framework::ResetDeltaTime()
{
	mDeltaTime = glfwGetTime();
	mTotalTime += mDeltaTime;
	glfwSetTime(0);
}