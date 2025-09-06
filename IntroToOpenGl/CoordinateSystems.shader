#Satyam vertex

#version 330 core

layout(loaction = 0) in vec3 aPos;

void main()
{
	gl_Position = aPos;
}

#Satyam fragment

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}