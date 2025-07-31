#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = aPos;
	TexCoord = aTexCoord;
}

#Satyam fragment

in vec2 TexCoord;

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture1, TexCoord);

}