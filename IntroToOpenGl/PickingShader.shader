#Satyam vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
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
#version 330 core

out vec4 FragColor;

uniform int objectID;

void main()
{
	// Encode ID into RGBA
	// R channel is enough for up to 255 objects
	// Can use more channels for larger scenes
	FragColor = vec4(float(objectID) / 255.0, 0,0,1.0);
}