#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aColor;

out	vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 perspectiveOnly;
uniform mat4 orthoOnly;
uniform bool zFightFix;

void main()
{
	color = aColor;
	
	if(zFightFix == false)
	{
		vec4 pos = projection * view * model * aPos;
		gl_Position = pos;
	}
	else
	{
		vec4 pos2 = perspectiveOnly * view * model * aPos;
		vec4 zPos = view * model * aPos;
		float z_Square = zPos.z * zPos.z;
		vec4 newPos = vec4(pos2.x, pos2.y, z_Square, pos2.w);
		vec4 feedPos = orthoOnly * newPos;
		gl_Position = vec4(feedPos.x, feedPos.y, feedPos.z, feedPos.w);
	}

}

#Satyam fragment

in vec3 color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(color, 1.0f);
}