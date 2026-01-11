#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aColor;

out	vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	color = aColor;
	
	vec4 pos = projection * view * model * aPos;
	gl_Position = pos;

}

#Satyam fragment

in vec3 color;
out vec4 FragColor;

uniform vec3 colorToBeShaded;

void main()
{
	FragColor = vec4(color * colorToBeShaded, 1.0f);
}