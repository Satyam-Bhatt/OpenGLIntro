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

// We need to pass in the texture object to the fragment shader
// For this GLSL has a built in data type for texture object called a sampler
// Depending on the texture object we have sampler2D, sampler3D, sampler1D
uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	// GLSL has built in texture function that takes in the first argument a texture sampler
	// and as its second argument the corresponding texture coordinate
	// The texture function then samples the corresponding color values as per the
	// texture paramenters (Filtering, Wrapping) we set earlier. 
	// The output of this fragment shader is the color of the texture at the texture coordinate
	FragColor = texture(ourTexture, TexCoord);
}