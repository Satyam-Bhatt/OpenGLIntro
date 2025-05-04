#Satyam vertex

#version 330 core
layout (location  = 0) in vec3 aPos;
uniform vec2 positionOffset;
out vec3 outPos;
void main()
{
	outPos = vec3(aPos.x + positionOffset.x, aPos.y + positionOffset.y, aPos.z);
	gl_Position = vec4(outPos.x, outPos.y, outPos.z, 1.0);
}

#Satyam fragment

#version 330 core
in vec3 outPos;
out vec4 color;

void main()
{
	color = vec4(outPos.x, outPos.y, outPos.z,1);
}