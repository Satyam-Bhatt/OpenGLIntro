#Satyam vertex

#version 330 core

layout(location = 0) in vec4 aPos; // The vertex position
layout(location = 1) in vec3 aColor; // The vertex color

out	vec3 color; // The color variable to be passed to the fragment shader

uniform mat4 model; // The model matrix
uniform mat4 view; // The view matrix
uniform mat4 projection; // The projection matrix. Has both perspective and orthographic components
uniform mat4 perspectiveOnly; // The projection matrix with only the perspective component
uniform mat4 orthoOnly; // The projection matrix with only the orthographic component
uniform bool zFightFix; // A boolean to enable or disable the z-fighting fix

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

		// As Z is divided by the W component we square it so that we don't loose depth information
		float z_Square = zPos.z * zPos.z;

		// We replace the Z value as the existing Z value is a linear approximation
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