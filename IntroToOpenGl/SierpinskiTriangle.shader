#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
uniform mat4 transform;
uniform vec3 color;

out vec3 colorOut;

void main()
{
	colorOut = color;

	vec4 pos = transform * aPos;
	gl_Position = pos;
}

#Satyam fragment

in vec3 colorOut;

out vec4 FragColor;

void main()
{
	FragColor = vec4(colorOut, 1.0f);
}