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

void main()
{
	vec2 uv = UV;
	vec2 newUV = (uv * 2 - 1) * cells;

	// Parsing x and y coordinates separately for non-linear effects to the graph
	if(nonLinearEffect1)
	{
		// Periodic distortion
		newUV = vec2(newUV.x + sin(Time) * sin(newUV.y), newUV.y + sin(Time) * cos(newUV.x));
	}

	if(nonLinearEffect2)
	{
		// Something cool
		newUV.x += sin(newUV.y * 6.28 + Time) * 0.2;
		newUV.y += cos(newUV.x * 6.28 + Time * 0.7) * 0.2;
	}

	if(nonLinearEffect3)
	{
		// All Ways Up
		newUV.x += sin(newUV.y * 4.0 + Time) * cos(newUV.x * 3.0 + Time * 0.5) * 0.3;
		newUV.y += cos(newUV.x * 4.0 + Time * 0.7) * sin(newUV.y * 3.0 + Time) * 0.3;
	}

	vec2 repeatedUV = fract(newUV);
	
	// SDF of the box just to make the lines a bit blurry and also have a hollow effect if fade < 0
	float box = 1-sdBox(repeatedUV, vec2(0.9));
	box = pow(box, fade);
	box = clamp(box, 0.0, 1.0);

	FragColor = vec4(box, box, box, 1.0f);
}