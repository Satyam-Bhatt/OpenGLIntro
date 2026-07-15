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
uniform int billBoardType;

void main()
{
	UV = aUV;

	if(billBoardType == 1)
	{}
	else if(billBoardType == 2)
	{}
	// // Approach 1 - Apply counter rotation so that when look at matrix rotates the world then this shader cancels out the rotation
	// mat3 viewRotationMatrix = mat3(view);
	// mat3 billboardRot = transpose(viewRotationMatrix);
	// vec3 rotatedPos = billboardRot * aPos; // Counter rotate
	// vec3 worldPos = rotatedPos + vec3(model[3]); // Adding so that it positions itself correctly in world space
	// vec4 pos = projection * view * vec4(worldPos, 1.0); // view tries to rotate but the worldPos is counter rotated(inverse)

	// // Approach 2 - SIMPLE & EFFICIENT
	// vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]); // X axis of the camera
	// vec3 camUp    = vec3(view[0][1], view[1][1], view[2][1]); // Y axis of the camera
	// vec3 billboardWorldPos = vec3(model[3]); // Pivot point in world space (last column of model matrix)
	// // aPos.x and aPos.y are vertex coords in object space
	// // camRight * aPos.x -> Basically cameraRight is a direction and aPos.x is a scaling factor (signed distance). It just offsets the vertex in cameraRight direction and gives us the worldSpace coords
	// // Same thing for camUp * aPos.y -> Which means moving the vertex in cameraUp direction by aPos.y signed distance.
	// // Adding these three offsets the vertex correctly around the pivot while making it face the camera.
	// vec3 worldPos = billboardWorldPos
	//               + camRight * aPos.x 
	//               + camUp    * aPos.y;
	// vec4 pos = projection * view * vec4(worldPos, 1.0);

	//  Approach 3 - Use the model matrix to preserve the rotation and scale
	// mat3 viewRotationMatrix = mat3(view);
	// mat3 billboardRot = transpose(viewRotationMatrix);
	// mat3 modelRotScale = mat3(model);
	// vec3 modelPos = modelRotScale * aPos;
	// vec3 rotatedPos = billboardRot * modelPos;
	// vec3 worldPos = rotatedPos + vec3(model[3]); 
	// vec4 pos = projection * view * vec4(worldPos, 1.0);

	// Approach 4 - Angle Clamping
    vec3 camForward = normalize(vec3(view[0][2], view[1][2], view[2][2]));

    vec3 worldUp = vec3(0.0, 1.0, 0.0);
    vec3 horizontalDir = normalize(vec3(camForward.x, 0.0, camForward.z)); // yaw direction as yaw is y axis

    float actualPitch = asin(clamp(camForward.y, -1.0, 1.0)); // Get the pitch with Y component. We clamp for safety
	float maxPitchAngle = radians(-30.0);
	float clampedPitch = clamp(actualPitch, -maxPitchAngle, maxPitchAngle);
    vec3 clampedForward = horizontalDir * cos(clampedPitch) + worldUp * sin(clampedPitch); // New vector with clamped values

    // New axis for the camera
    vec3 camRight = normalize(cross(worldUp, clampedForward));
    vec3 camUp    = cross(clampedForward, camRight);

    vec3 billboardWorldPos = vec3(model[3]);
    vec3 worldPos = billboardWorldPos
                  + camRight * aPos.x
                  + camUp    * aPos.y;
	 vec4 pos = projection * view * vec4(worldPos, 1.0);

	 //WHAT NEXT?
	 // - Normal Texture for 3D
	 // - A compute shader to spawn then 
	 // - Fade at base for blending
	 // 

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