// We have set glDrawBuffer(GL_NONE) that is why we don't need to output anythin in the fragment stage
// Also this shader is being used by FBO of type DEPTH, so the fixed function depth test/writes stores it into the FBO's Depth Attachment
// Depth is written by interpolating z/w values and remapping to 0-1 that we get from the vertices by the rasterizer
#Satyam vertex

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

uniform mat4 model;
// Sending Light Space is better than sending projection * view matrix as CPU does this once every frame and if we send it to the GPU, it would do projection * view once every vertex which is quite costly
// We can also multiply the model matrix on the CPU side and send that as well. That also cuts down the cost but there are tradeoffs as it breaks instancing. As we won't have model matrix we won't be able to get the world position.
uniform mat4 lightSpace;

// After we write the gl_Position the rasterizer takes care of the fragment and their depth while output merger stores it. These 2 are not programmable
void main()
{
	gl_Position = lightSpace * model * vec4(aPos, 1.0);
}

#Satyam fragment
#version 330 core

// fragment shader runs but is not needed in this case
// It can override the depth by writing  gl_FragDepth
void main()
{
}