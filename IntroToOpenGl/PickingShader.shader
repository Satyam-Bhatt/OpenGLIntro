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

// Integer has 32 bits(4 bytes)
// A color has 4 channels each channel ranges from 0-255 
// So we split the integer in 4 parts as per the bytes in each color channel
// R channel - 0-7 bits (byte 0)
// G channel - 8-15 bits (byte 1)
// B channel - 16-23 bits (byte 2)
// A channel - 24-31 bits (byte 3)
//
// 0xFF = 255 -> in bits 00000000 00000000 00000000 11111111
// & operator helps in masking out the last 8 bits as 255 has the first 3 bytes as 00000000 and last byte is 11111111
//
// >> right shift -> It moves the bits to the right. It is used to sample the next byte 
// 00000001 00000000  (256 in 16 bits)
// >>         8          shift right by 8
// ──────────────────
// 00000000 00000001  (1) ← the 1 that was in byte 1 is now in byte 0
//
// Doing this helps us to encode huge numbers in a vector 4
// We divide by 255 to convert it into a color as color values are between 0-1
//
// EXAMPLE
//
// int id = 305419896;
//
// Extract byte 0 (R) — lowest 8 bits
// AND with 0xFF to mask out everything except lowest 8 bits
// id         = 00010010 00110100 01010110 01111000
// 0xFF       = 00000000 00000000 00000000 11111111
// id & 0xFF  = 00000000 00000000 00000000 01111000  = 120
// R = float(120) / 255.0 = 0.471
//
// Extract byte 1 (G) — bits 8-15
// First shift right by 8 to bring byte 1 down to position 0
// id >> 8    = 00000000 00010010 00110100 01010110  = byte1 is now at bottom
// & 0xFF     = 00000000 00000000 00000000 01010110  = 86
// G = float(86) / 255.0 = 0.337
//
// Extract byte 2 (B) — bits 16-23
// Shift right by 16 to bring byte 2 down to position 0
// id >> 16   = 00000000 00000000 00010010 00110100  = byte2 is now at bottom
// & 0xFF     = 00000000 00000000 00000000 00110100  = 52
// B = float(52) / 255.0 = 0.204
//
// Extract byte 3 (A) — bits 24-31
// Shift right by 24 to bring byte 3 down to position 0
// id >> 24   = 00000000 00000000 00000000 00010010  = byte3 is now at bottom
// & 0xFF     = 00000000 00000000 00000000 00010010  = 18
// A = float(18) / 255.0 = 0.071
//
// FragColor = vec4(0.471, 0.337, 0.204, 0.071);
//
// --------
//
// NOTE: Currently we are writing 0-255 only. If the value goes above 255 then there would be data loss as GPU clamps GL_UNSIGNED_BYTE between 0-1(float) or 0-255(byte). So if something is more than 1 or 255 like 1.5 or 300 it would be read as 1 or 255 
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