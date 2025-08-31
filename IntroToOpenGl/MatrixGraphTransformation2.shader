#Satyam vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
out	vec2 vTexCoords;

void main()
{
	vTexCoords = aTexCoords;
	gl_Position = vec4(aPos,0,1);
}

#Satyam fragment

in vec2 vTexCoords;
out vec4 FragColor;
uniform float cells;
uniform vec3 lineColor;

float sdBox( vec2 p, vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

void main()
{
	vec2 uv = vTexCoords;
	vec2 newUV = (uv * 2 - 1) * cells;

	vec2 repeatedUV = fract(newUV);
	float t = 0.04;
	float box = float(repeatedUV.x>(t) && repeatedUV.x<(1.0-t) && repeatedUV.y<(1.0-t) && repeatedUV.y>(t));
	box = 1 - box;
	vec3 col = lineColor * box;

	// FragColor = vec4(col, box);

	vec2 centerLineUV = (vTexCoords * 2 - 1) * 2;
	centerLineUV = fract(centerLineUV);
	float t2 = 0.025;
	float forCenter = float(centerLineUV.x>(t2) && centerLineUV.x<(1.0-t2) && centerLineUV.y<(1.0-t2) && centerLineUV.y>(t2));
	forCenter = 1 - forCenter;
	vec3 col2 = lineColor * 0.5 * forCenter;

	vec3 col4 = mix(col, col2, forCenter);

	FragColor = vec4(col4, box + forCenter);

}
