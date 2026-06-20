#Satyam vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 pos = projection * view * model * vec4(aPos,1.0);
	gl_Position = pos;
}

#Satyam fragment
#version 330 core

out vec4 FragColor;

uniform int objectID;

void main()
{
    int id = objectID;  // explicit int
    FragColor = vec4(
        float((id)       & 0xFF) / 255.0,
        float((id >> 8)  & 0xFF) / 255.0,
        float((id >> 16) & 0xFF) / 255.0,
        float((id >> 24) & 0xFF) / 255.0
    );
}