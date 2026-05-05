// Receve a color as uniform and color the entire thing like that
// I hate getting things as uniform because it means so many calls between cpu and gpu that are far apart. I wish they were close friends
#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(loaction = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 pos = projection * view * model * vec4(aPos,1.0);
	gl_Position = pos;
}

#Satyam fragment

out vec4 FragColor;

uniform vec4 color;

void main()
{
	FragColor = color;
}
