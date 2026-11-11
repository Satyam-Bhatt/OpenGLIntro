// TODO: What is happening here?
#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

uniform mat4 model;
uniform mat4 lightSpace; // TODO: Why not projection * view * model? Why is it just lightSpace * model?

void main()
{
	gl_Position = lightSpace * model * vec4(aPos, 1.0);
}

#Satyam fragment
#version 330 core

void main()
{
}