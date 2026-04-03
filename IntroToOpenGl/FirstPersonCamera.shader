#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTextCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoord = aTextCoord;
	vec4 pos = projection * view * model * vec4(aPos,1.0);
	gl_Position = pos;
}

#Satyam fragment

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D myTexture;
uniform bool useColor;

void main()
{
	if(useColor)
		FragColor = vec4(0.5, 0.5, 0.5,1);
	else
		FragColor = texture(myTexture, TexCoord);
}