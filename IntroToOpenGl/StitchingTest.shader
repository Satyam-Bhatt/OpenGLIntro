#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;

out vec4 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	pos = aPos;
	vec4 pos = projection * view * model * aPos;
	gl_Position = pos;
}

#Satyam fragment

in vec4 pos;
out vec4 FragColor;

void main()
{
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = pos;
}