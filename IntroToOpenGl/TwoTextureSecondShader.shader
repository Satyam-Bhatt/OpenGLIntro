#Satyam vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
}

#Satyam fragment

#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D texture3;

void main()
{
	vec4 colTex = texture(texture3, TexCoord);
	FragColor = (vec4(ourColor, colTex.a) - 0.3) * colTex;
}