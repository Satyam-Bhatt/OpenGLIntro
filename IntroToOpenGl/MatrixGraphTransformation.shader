#Satyam vertex
#version 330 core

layout (location = 0) in vec2 aPos;
uniform mat2 mat;

void main()
{
	vec2 pos = aPos * mat;
	gl_Position = vec4(pos,0,1);
}

#Satyam fragment

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}