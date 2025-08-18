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

uniform float time;

void main()
{
	vec3 colorOut2 = vec3(colorOut.r * (sin(time * 2) * 0.5 + 0.5), colorOut.g * (cos(time * 2) * 0.5 + 0.5), colorOut.b * (sin(time * 2) * 0.8 + 0.8));
	FragColor = vec4(colorOut2, 1.0f);
}