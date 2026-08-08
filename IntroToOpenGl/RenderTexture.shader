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
	vec4 pos = projection * view * model * vec4(aPos, 1.0);
	gl_Position = pos;
}

#Satyam fragment
#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform vec4 _Color = vec4(1.0, 1.0, 1.0, 1.0); 
uniform sampler2D myTexture;
uniform vec4 tillingOffset = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec2 uv = UV * vec2(tillingOffset.x, tillingOffset.y) + vec2(tillingOffset.z, tillingOffset.w);
	FragColor = _Color * texture(myTexture, uv);
}