#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
//layout(location = 1) in vec2 aTexCoord;

// Get the transformation matrix
//uniform mat4 transform;

//out vec2 TexCoord;

void main()
{
	// Apply the transformation to the vertex postiom
	//vec4 pos = transform * aPos;
	//gl_Position = pos;
	
	gl_Position = aPos;

	//TexCoord = aTexCoord;
}

#Satyam fragment

//in vec2 TexCoord;

//uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.5f, 0.5f, 0.2f, 1.0f);

}