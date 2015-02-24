#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm\glm.hpp"

#include "framework/Sprite.h"
#include "framework/FontManager.h"
#include "framework/Animation.h"

#include <vector>

enum KEY_CODE
{
	A = GLFW_KEY_A,
	S = GLFW_KEY_S,
	D = GLFW_KEY_D,
	W = GLFW_KEY_W,
	SPACE = GLFW_KEY_SPACE,
	ESC = GLFW_KEY_ESCAPE
};


class Framework
{
public:
	Framework(){};

	/**
	Called to initialize the framework, creating a window of given height, width and title.\n
	__This function should be the first called when using the framework.__
	####paramaters
	`screenWidth` - Width of window created by framework.\n
	`screenHeight` - Height of window created by framework\n
	`title` - Title of window created by framework.
	*/
	void Initialize(const int screenWidth, const int screenHeight, const char* title);

	/**
	Called to do any necessary cleanup before exiting the program.

	__This function should be the last called to cleanup the framework before exiting program.__
	*/
	void Shutdown();

	/**
	Called during game loop to render drawn objects to thy screen.
	####returns
	Returns true if the framework updated with no problems, else returns false.\n
	_Example:_
	~~~
	Framework frk;
	frk.Initialize(100, 100, "Menu Title");
	do
	{
		...

	}while(frk.UpdateFramework);
	~~~
	
	*/
	bool UpdateFramework();


	/**
	Create a non-animated sprite with a given texture to be moved and drawn on the screen.\n
	####parameters
	`width` - Width of the object in pixels.\n
	`height` - Height of the object in pixels.\n
	`texture` - Filename of the image to be used as a texture of this object. _The image will be shrunk/stretched to fit the
	size of the object._\n
	`isCentered` - The origin of the object (the point where the position is set) is in the center of the object if true, otherwise is in the lower-left corner.\n
	####returns
	A unique sprite ID to be used when moving, drawing within framework.
	
	*/
	unsigned int CreateSprite(const float width, const float height, const char* texture, const bool isCentered);

	/**
	Set the UV coordinates for the given sprites sprite sheet.
	####parameters
	`spriteID` - ID returned from `CreateSprite()` for sprite want to set UV coordinates.\n
	`minX` - Minimum x value of UV rectangle.\n
	`minY` - Minimum y value of UV rectangle.\n
	`maxX` - Maximum x value of UV rectangle.\n
	`maxY` - Maximum y value of UV rectagle.\n
	*/
	void SetSpriteUV(const unsigned int spriteID, const float minX, const float minY, const float maxX, const float maxY);
	
	/**
	Set the scale (size) of the given sprite in pixels.
	####parameters
	`spriteID` - ID returned from `CreateSprite()` for sprite to set.\n
	`scaleX` - The width of the sprite in pixels.\n
	`scaleY` - The height of the sprite in pixels.\n
	*/
	void SetSpriteScale(unsigned int spriteId, float scaleX, float scaleY);

	/**
	Move the given sprite to the given pixel position coordinates on the screen.
	####parameters
	`spriteID` - ID returned from `CreateSprite()` for sprite to set.\n
	`xPosition` - position on x axis to set the sprite.\n
	`yPosition` - position on y axis to set the sprite.\n
	*/
	void MoveSprite(unsigned int spriteID, const float xPosition, const float yPosition);

	/**
	Draw the given sprite on the screen.
	####parameters
	`spriteID` - ID returned from `CreateSprite()` for sprite to draw.\n
	*/
	void DrawSprite(unsigned int spriteID);

	void DrawSprite(unsigned int spriteID, glm::vec4& color);

	/**
	Create an animated sprite with a given texture sprite sheet to be moved and drawn on the screen.\n
	####parameters
	`width` - Width of the object in pixels.\n
	`height` - Height of the object in pixels.\n
	`spriteSheetDataFile` - Filename of the sprite sheet description file to be used as a texture of this object. _The image will be shrunk/stretched to fit the
	size of the object._\n
	####returns
	A unique animated sprite ID to be used when moving, drawing within framework.

	*/
	unsigned int CreateAnimation(const float width, const float height, const char* spriteSheetDataFile);

	/**
	Flip the direction of the given animated sprite along y axis.
	####parameters
	`animationId` - ID returned from `CreateAnimation()`
	*/
	void AnimationFlipDirection(unsigned int animationId);

	/**
	Set the gievn animation's animation state.
	####parameters
	`animationId` - ID returned from `CreateAnimation()`.
	`a_state` - The animation state to set.
	*/
	void SetAnimationState(const unsigned int animationId, const char* a_state);

	/**
	Set the given animation's position on the screen in pixels.
	####parameters
	`animationId` - ID returned from `CreateAnimation()`.\n
	`xPosition` - Position on x axis to set the animation in pixels.\n
	`yPosition` - Position on y axis to set the animation in pixels.\n
	*/
	void MoveAnimation(unsigned int animationID, float xPosition, float yPosition);

	/**
	Draw the given animation on the screen.
	####parameters
	`animationID` - ID returned from `CreateAnimation()` for sprite to draw.\n
	*/
	void DrawAnimation(unsigned int animationID);


	/**
	Draw text to the screen.
	####parameters
	`text` - Text to draw to the string.\n
	`xPosition` - x position to start drawing text.\n
	`yPosition` - y position to start drawing text.
	*/
	void DrawText(const char* text, const float xPosition, float yPosition);

	/**
	Set backgound color to given RGBA values.\n
	__Values are expected to be from 0 to 1.__
	####parameters
	`r` - Red value from 0 to 1.\n
	`g` - Green value from 0 to 1.\n
	`b` - Blue value from 0 to 1.\n
	`a` - Alpha value from 0 to 1. __note: 0 is totally transparent, while 1 equates total opacity.__
	*/
	void SetBackgroundColor(const float r, const float g, const float b, const float a);

	/**
	Call beginning of main game loop to clear the screen each frame.
	*/
	void ClearScreen();

	/**
	Call to poll the keyboard for user input.
	####parameters
	`a_key` - Key you want to poll.\n
	####returns
	Returns true if given key is being pressed while being polled, else returns false.	
	*/
	bool IsKeyDown(KEY_CODE a_key);

	/**
	Returns the time in seconds from the last frame.
	*/
	double GetDeltaTime();

	/**
	Returns the total time in seconds the application has been running.
	*/
	double GetTotalTime();
private:
	GLFWwindow* mWindow;
	glm::vec4 mBackgroundColor;
	double mTotalTime;
	double mDeltaTime;
	std::vector<Sprite*> mSpriteList;
	std::vector<Animation*> mAnimationList;
	FontManager mFontMan;

	void ResetDeltaTime();
};

#endif