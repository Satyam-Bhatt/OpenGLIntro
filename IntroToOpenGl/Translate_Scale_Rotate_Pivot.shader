#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;

void main()
{
	gl_Position = aPos;
}

#Satyam fragment

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}