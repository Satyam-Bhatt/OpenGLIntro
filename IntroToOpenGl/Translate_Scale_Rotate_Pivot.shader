#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aUV;

out vec2 UV;

void main()
{

	UV = aUV;
	gl_Position = aPos;
}

#Satyam fragment

in vec2 UV;

out vec4 FragColor;

void main()
{
	vec2 uv = UV;
	vec2 center = vec2(0.5f, 0.5f);
	float radius = 0.3f;
	float len = length(uv - center) - radius;
	len = step(len, 0.0f);
	vec3 col = vec3(1.0f, 0.0f, 0.0f);

	vec2 uv2 = UV * 2 - 1; 
	float dist = length(uv2);
	dist = 1 - dist;
	dist = clamp(dist,0,1);
    dist = pow(dist, 1);
	col = col * dist;
	FragColor = vec4(col, len);

	//FragColor = vec4(col* dist , len);
}