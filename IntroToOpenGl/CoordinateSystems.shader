#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;

uniform mat4 rot;

void main()
{
	vec4 pos = rot * aPos;
	gl_Position = pos;
}

#Satyam fragment

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}