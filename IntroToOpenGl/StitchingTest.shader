#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aColor;

out vec4 pos;
out	vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 perspectiveOnly;
uniform mat4 orthoOnly;

void main()
{
	pos = aPos;
	color = aColor;
	vec4 pos = projection * view * model * aPos;
	//gl_Position = pos;

	float z_Square = aPos.z * aPos.z;
	vec4 pos2 = perspectiveOnly * view * model * aPos;
	vec4 newPos = vec4(pos2.x, pos2.y, z_Square, pos2.w);
	vec4 feedPos = orthoOnly * newPos;
	gl_Position = feedPos;
}

#Satyam fragment

in vec4 pos;
in vec3 color;
out vec4 FragColor;

void main()
{
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//FragColor = pos;
	FragColor = vec4(color, 1.0f);
}