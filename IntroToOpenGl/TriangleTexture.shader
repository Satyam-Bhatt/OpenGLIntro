#Satyam vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	ourColor = aCol;
	texCoord = aTexCoord;
}

#Satyam fragment

#version 330 core
in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;
void main()
{
	FragColor = texture(ourTexture, vec2(texCoord.s, texCoord.t));
}