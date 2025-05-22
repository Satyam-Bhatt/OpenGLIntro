//IntroTexture.shader

#Satyam vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}

#Satyam fragment

#version 330 core
in vec3	ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture, TexCoord);
}