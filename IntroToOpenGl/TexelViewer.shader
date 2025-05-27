#Satyam vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	TexCoords = aTexCoords;
}

#Satyam fragment

in vec2 TexCoords;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture, TexCoords);
	//FragColor = vec4(TexCoords, 0.0, 1.0);
}