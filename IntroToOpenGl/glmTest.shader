#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
	gl_Position = aPos;
	TexCoord = aTexCoord;
}

#Satyam fragment

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);

}