#Satyam vertex

#version 330 core
layout (location  = 0) in vec3 aPos;
uniform float valueOverTime = 0.0f;
void main()
{
	float positionY = aPos.y;
	float newPostionY = mix(positionY, -positionY, valueOverTime);
	gl_Position = vec4(aPos.x, newPostionY, aPos.z, 1.0);
}

#Satyam fragment

#version 330 core
out vec4 color;
uniform float time;
uniform float valueOverTime = 0.0f;

vec3 Sunset(float t) {
   vec3 a = vec3(0.6, 0.6, 0.65);    // Higher baseline for subtlety
    vec3 b = vec3(0.1, 0.15, 0.2);    // Reduced amplitude for less variation
    vec3 c = vec3(0.7, 0.8, 0.8);     // Adjusted frequency
    vec3 d = vec3(0.5, 0.6, 0.7);     // Phase adjusted for gentle shifts
    
    return a + b * cos(6.28318 * (c * t + d));
}

vec3 Ocean(float t) {
    vec3 a = vec3(0.65, 0.62, 0.58);  // Neutral base
    vec3 b = vec3(0.08, 0.1, 0.12);   // Very small amplitude
    vec3 c = vec3(0.5, 0.5, 0.4);     // Lower frequency
    vec3 d = vec3(0.2, 0.1, 0.0);     // Phase for earth tones
    
    return a + b * cos(6.28318 * (c * t + d));
}


void main()
{
    float t = time * 0.3f;
    vec3 sunsetColor = Sunset(t);
	vec3 oceanColor = Ocean(t);
	vec4 colorLerp = mix(vec4(sunsetColor, 1.0f), vec4(oceanColor, 1.0f), valueOverTime);

	//vec4 colorLerp = mix(vec4(0.059f, 0.545f, 0.553f, 1.0f), vec4(0.949f, 0.722f, 0.502f, 1.0f), valueOverTime);
	color = colorLerp;
}