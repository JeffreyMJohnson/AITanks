#ifndef _SPRITE_H_
#define _SPRITE_H_

#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"

#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "SOIL/SOIL.h"

#include "Globals.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>



extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Sprite
{
public:
	GLFWwindow* window;
	GLuint vao;//vertex array object
	GLuint vbo;//vertex buffer object
	GLuint uvo;//buffer for UV coordinates

	float* vertices;
	GLuint textureID;
	GLuint uniTextureSample;
	GLuint uniColor;
	GLuint shaderProgram;
	glm::mat4 transform;
	glm::vec3 translation, scale;
	float rotZ;
	//glm::vec2 position;
	//float width, height;

	GLint uniMVP;

	glm::mat4 viewMatrix;
	glm::mat4 cameraMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;


	Sprite();

	~Sprite();

	void Initialize(float a_width, float a_height, const char* texturePath, bool centeredOrigin);

	void SetTranslation(const glm::vec3 a_Translation);

	void SetSpriteUV(float minX, float minY, float maxX, float maxY);

	void SetScale(const float scaleX, const float scaleY);

	void SetRotation(const float angle);

	void Draw();

	void Draw(glm::vec4& a_color);

	void UpdateTransform();

	float DegreeToRadians(float degrees);

	void CreateShaderProgram();

	GLuint CreateShader(GLenum a_ShaderType, const char* a_ShaderFile);

	GLuint CreateProgram(const char* a_vertex, const char* a_frag);

};

#endif