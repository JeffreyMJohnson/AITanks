#version 150

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D TextureSample;
uniform vec4 color;

void main()
{
	outColor = texture(TextureSample, TexCoord) * color;
	//outColor = vec4(1,1,1,1);
}