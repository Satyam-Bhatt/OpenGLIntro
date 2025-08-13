#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;

// Get the transformation matrix
uniform mat4 transform;

//out vec2 TexCoord;

void main()
{
	// Apply the transformation to the vertex postiom
	vec4 pos = transform * aPos;
	gl_Position = pos;
}

#Satyam fragment

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.5f, 1.0f, 0.2f, 1.0f);

}