#Satyam vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	UV = aUV;
	vec4 pos = projection * view * model * vec4(aPos,1.0);
	gl_Position = pos;
}

#Satyam fragment
#version 330 core

in vec2 UV;
out vec4 FragColor;

float sdBox( vec2 p, vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

void main()
{
	vec2 uv = UV;
	vec2 newUV = (uv * 2 - 1) * 1000;

	vec2 repeatedUV = fract(newUV);
	
	// SDF of the box just to make the lines a bit blurry and also have a hollow effect if fade < 0
	float box = 1-sdBox(repeatedUV, vec2(0.9));
	box = pow(box, 3);
	box = clamp(box, 0.0, 1.0);

	FragColor = vec4(1, 1, 1, 1.0 - box);
}