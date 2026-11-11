// TODO: Explain in detail why we don't require a fragment here - because its bound to FBO with format Depth24 and depth 24 only needs Vertex position.z for depth buffer there is no color output in this shader as FBO format of Depth 24 doesn't require it
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