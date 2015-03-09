#include "framework/Sprite.h"

Sprite::Sprite()
{

}

Sprite::~Sprite()
{

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &uvo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);
	delete[] vertices;
}



void Sprite::Initialize(float a_width, float a_height, const char* texturePath, bool centeredOrigin)
{

	//shaderProgram = a_shaderProgram;
	CreateShaderProgram();
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	vertices = new float[16];
	if (centeredOrigin)
	{
		vertices[0] = -.5;//top-left
		vertices[1] = .5;
		vertices[2] = 0;
		vertices[3] = 0;

		vertices[4] = .5;//top-right
		vertices[5] = .5;
		vertices[6] = 1;
		vertices[7] = 0;

		vertices[8] = .5;//bottom-right
		vertices[9] = -.5;
		vertices[10] = 1;
		vertices[11] = 1;

		vertices[12] = -.5;//bottom-left
		vertices[13] = -.5;
		vertices[14] = 0;
		vertices[15] = 1;
	}
	else
	{
		vertices[0] = 0;//top-left
		vertices[1] = 1;
		vertices[2] = 0;
		vertices[3] = 0;

		vertices[4] = 1;//top-right
		vertices[5] = 1;
		vertices[6] = 1;
		vertices[7] = 0;

		vertices[8] = 1;//bottom-right
		vertices[9] = 0;
		vertices[10] = 1;
		vertices[11] = 1;

		vertices[12] = 0;//bottom-left
		vertices[13] = 0;
		vertices[14] = 0;
		vertices[15] = 1;
	}



	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint positionAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	GLint texCoordAttrib = glGetAttribLocation(shaderProgram, "texCoord");
	glEnableVertexAttribArray(texCoordAttrib);
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));


	uniMVP = glGetUniformLocation(shaderProgram, "MVP");

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int imageWidth, imageHeight;
	unsigned char* image = SOIL_load_image(texturePath, &imageWidth, &imageHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	uniTextureSample = glGetUniformLocation(shaderProgram, "TextureSample");
	glUniform1i(uniTextureSample, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	uniColor = glGetUniformLocation(shaderProgram, "color");
	glm::vec4 mColor = glm::vec4(1, 1, 0, 1);
	glUniform4f(uniColor, mColor.r, mColor.g, mColor.b, mColor.a);

	transform = glm::mat4(1);
	translation = glm::vec3(0, 0, 0);

	scale = glm::vec3(a_width, a_height, 1);
	rotZ = 0;

	viewMatrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::ortho(0.0f, 1024.0f, 0.0f, 768.0f);



	UpdateTransform();

	//clear state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::SetTranslation(const glm::vec3 a_Translation)
{
	translation = a_Translation;
	UpdateTransform();
}

void Sprite::SetSpriteUV(float minX, float minY, float maxX, float maxY)
{

	//top-left
	vertices[2] = minX;
	vertices[3] = minY;
	//top-right
	vertices[6] = maxX;
	vertices[7] = minY;
	//bottom-right
	vertices[10] = maxX;
	vertices[11] = maxY;
	//bottom-left
	vertices[14] = minX;
	vertices[15] = maxY;

	//set state
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//load data into gpu buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertices, GL_STATIC_DRAW);

	//clear state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::SetScale(const float scaleX, const float scaleY)
{
	scale.x = scaleX;
	scale.y = scaleY;
	UpdateTransform();
}

void Sprite::SetRotation(const float angle)
{
	rotZ = angle;
	UpdateTransform();
}

void Sprite::Draw()
{
	//set state
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//send uniform to shader
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

	glm::vec4 mColor = glm::vec4(1, 1, 1, 1);
	glUniform4f(uniColor, mColor.r, mColor.g, mColor.b, mColor.a);

	//draw it
	glDrawArrays(GL_QUADS, 0, 4);


	//clear state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Sprite::Draw(glm::vec4& a_color)
{
	//set state
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//send uniform to shader
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

	glUniform4f(uniColor, a_color.r, a_color.g, a_color.b, a_color.a);

	//draw it
	glDrawArrays(GL_QUADS, 0, 4);


	//clear state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::UpdateTransform()
{

	glm::mat4 translationMatrix(1.0f);
	translationMatrix = glm::translate(glm::mat4(), translation);
	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, scale);
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, rotZ, glm::vec3(0, 0, 1));
	transform = translationMatrix * rotationMatrix * scaleMatrix;

	modelViewProjectionMatrix = projectionMatrix * viewMatrix * transform;

	//set state
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

	//clear state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float Sprite::DegreeToRadians(float degrees)
{
	return degrees * RADIAN_CONVERSION;
}

void Sprite::CreateShaderProgram()
{
	shaderProgram = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\TexturedFragmentShader.glsl");
	//shaderProgram = CreateProgram(".\\source\\VertexShader.glsl", ".\\source\\FlatFragmentShader.glsl");
}

GLuint Sprite::CreateShader(GLenum a_ShaderType, const char* a_ShaderFile)
{
	std::string shaderCode;
	//open shader file
	std::ifstream shaderStream(a_ShaderFile);
	//if that worked ok, load file line by line
	if (shaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(shaderStream, line))
		{
			shaderCode += "\n" + line;
		}
		shaderStream.close();
	}

	//convert to cstring
	char const* shaderSourcePointer = shaderCode.c_str();

	//create shader ID
	GLuint shader = glCreateShader(a_ShaderType);
	//load source code
	glShaderSource(shader, 1, &shaderSourcePointer, NULL);

	//compile shader
	glCompileShader(shader);

	//check for errors
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

		const char* shaderType = NULL;
		switch (a_ShaderType)
		{
		case GL_VERTEX_SHADER:
			shaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "fragment";
			break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", shaderType, infoLog);
		delete[] infoLog;
	}

	return shader;

}

GLuint Sprite::CreateProgram(const char* a_vertex, const char* a_frag)
{

	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, a_vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, a_frag));

	//create shader program ID
	GLuint program = glCreateProgram();

	//attach shaders
	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glAttachShader(program, *shader);
	}

	//link program
	glLinkProgram(program);

	//check for link errors and output them
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);

		fprintf(stderr, "Linker failure: %s\n", infoLog);
		delete[] infoLog;
	}

	for (auto shader = shaderList.begin(); shader != shaderList.end(); shader++)
	{
		glDetachShader(program, *shader);
		glDeleteShader(*shader);
	}
	return program;
}