// For rendering a texture/image on different faces depending on texture coordinates
#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	UV = aUV;

	// Approach 1
	// mat3 viewRotationMatrix = mat3(view);
	// mat3 billboardRot = transpose(viewRotationMatrix);
	// vec3 rotatedPos = billboardRot * aPos;
	// vec3 worldPos = rotatedPos + vec3(model[3]);
	// vec4 pos = projection * view * vec4(worldPos, 1.0);

	// Approach 2
	mat3 viewRotationMatrix = mat3(view);
	vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]); 
	vec3 camUp    = vec3(view[0][1], view[1][1], view[2][1]);
	vec3 billboardWorldPos = vec3(model[3]);

	vec3 worldPos = billboardWorldPos
	              + camRight * aPos.x
	              + camUp    * aPos.y;
	vec4 pos = projection * view * vec4(worldPos, 1.0);

	//vec4 pos = projection * view * model * vec4(aPos, 1.0);
	gl_Position = pos;
}

#Satyam fragment
#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
	FragColor = texture(myTexture, UV);
}