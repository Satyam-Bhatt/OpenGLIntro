#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

out vec2 UV;

void main()
{
	UV = aUV;
	vec4 pos = vec4(aPos, 1.0);
	gl_Position = pos;
}

#Satyam fragment
#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D myTexture0;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform sampler2D myTexture3;
uniform float time;

void main()
{
	vec4 t0 = texture(myTexture0, UV);
	vec4 t1 = texture(myTexture1, UV);
	vec4 t2 = texture(myTexture2, UV);
	vec4 t3 = texture(myTexture3, UV);

	float selectVal = mod(floor(time), 4);
	vec4 outputTex;

	if(selectVal == 0) outputTex = t0;
	if(selectVal == 1) outputTex = t1;
	if(selectVal == 2) outputTex = t2;
	if(selectVal == 3) outputTex = t3;

	FragColor = outputTex;
}