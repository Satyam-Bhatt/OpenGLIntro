#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

out vec2 UV;
out vec4 fragPosLightSpace;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 lightSpace = mat4(1.0);

void main()
{
	UV = aUV;
	fragPosLightSpace = lightSpace * model * vec4(aPos, 1.0); // TODO:What???
	vec4 pos = projection * view * model * vec4(aPos, 1.0);
	gl_Position = pos;
}

#Satyam fragment
#version 330 core

in vec2 UV;
in vec4 fragPosLightSpace;

out vec4 FragColor;

uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0); 
uniform sampler2D myTexture;
uniform vec4 tillingOffset= vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D shadowMap;

// TODO: What???
float CalcShadow(vec4 posLS)
{
	vec3 p = posLS.xyz / posLS.w;   // for ortho, w is 1, but this keeps it general
	p = p * 0.5 + 0.5;              // -1..1 -> 0..1

	if (p.z > 1.0) return 0.0;      // beyond the light's far plane: lit

	float closest = texture(shadowMap, p.xy).r;
	return (p.z - 0.005 > closest) ? 1.0 : 0.0;
}

// TODO: More expensive probably a cheaper billenear would be better
float CalcShadowPCF(vec4 posLS)
{
	vec3 p = posLS.xyz / posLS.w;
	p = p * 0.5 + 0.5;

	// Outside the light's frustum: lit
	if (p.x < 0.0 || p.x > 1.0 || p.y < 0.0 || p.y > 1.0 || p.z > 1.0)
		return 0.0;

	vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
	float shadow = 0.0;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float closest = texture(shadowMap, p.xy + vec2(x, y) * texelSize).r;
			shadow += (p.z - 0.005 > closest) ? 1.0 : 0.0;
		}
	}

	return shadow / 25.0;
}

void main()
{
	vec2 uv = UV * vec2(tillingOffset.x, tillingOffset.y) + vec2(tillingOffset.z, tillingOffset.w);
	float shadow = CalcShadowPCF(fragPosLightSpace); // TODO: What???
	FragColor = color * texture(myTexture, uv);
	FragColor *= (1.0 - shadow); // TODO: What???
}