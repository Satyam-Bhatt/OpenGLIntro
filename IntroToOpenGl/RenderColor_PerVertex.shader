// To Render different colors on different faces. We receive color as an attribute
#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(loaction = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	color = aColor;

	glPosition = projection * view * model * vec4(aPos, 1.0);
}

#Satyam fragment

in vec4 color;
out vec4 FragColor;

void main()
{
	FragColor = color;
}