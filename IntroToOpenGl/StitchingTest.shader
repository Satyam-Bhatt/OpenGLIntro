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
		// This fixes Z fighting as we replace non linear Z values with linear values. Though it mathematically incorrect but it does the job

		// Find the position of the vertex when it goes from a square frusturm to cuboid
		vec4 positionInTheCuboid = perspectiveOnly * view * model * aPos;

		// Z position after being affected by model and view matrix
		vec4 zPos = view * model * aPos;

		// As Z is divides by W we square it so that we don't loose depth information
		float z_Square = zPos.z * zPos.z;

		// We replace the Z value as the existing Z value is non linear because of the quadratic equation m1 Z + m2 = Z^2
		positionInTheCuboid = vec4(positionInTheCuboid.x, positionInTheCuboid.y, z_Square, positionInTheCuboid.w);

		// We multiply this with the orthographic matrix to reposition and scale the cuboid
		vec4 feedPos = orthoOnly * positionInTheCuboid;

		// gl_Position automatically converts homogenous coordinates to cartesian by dividing X,Y and Z by W
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