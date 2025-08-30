#Satyam vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
out	vec2 vTexCoords;
uniform mat2 mat;

void main()
{
	vec2 pos = aPos * mat;
	vTexCoords = aTexCoords;
	gl_Position = vec4(pos,0,1);
}

#Satyam fragment

in vec2 vTexCoords;
out vec4 FragColor;
uniform float Time;
uniform float fade;
uniform float cells;

float sdBox( vec2 p, vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

void main()
{
	vec2 uv = vTexCoords;
	vec2 newUV = (uv * 2 - 1) * cells;

	// Periodic distortion
	newUV = vec2(newUV.x + sin(Time) * sin(newUV.y), newUV.y + sin(Time) * cos(newUV.x));

	// Something cool
	// newUV.x += sin(newUV.y * 6.28 + Time) * 0.2;
	// newUV.y += cos(newUV.x * 6.28 + Time * 0.7) * 0.2;

	// All Ways Up
	// newUV.x += sin(newUV.y * 4.0 + Time) * cos(newUV.x * 3.0 + Time * 0.5) * 0.3;
	// newUV.y += cos(newUV.x * 4.0 + Time * 0.7) * sin(newUV.y * 3.0 + Time) * 0.3;

	vec2 repeatedUV = fract(newUV);
	
	float box = 1-sdBox(repeatedUV, vec2(0.9));
	box = pow(box, fade);
	box = clamp(box, 0.0, 1.0);

	FragColor = vec4(box, box, box, 1.0f);

}
