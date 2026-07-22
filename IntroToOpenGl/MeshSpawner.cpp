#include "MeshSpawner.h"

MeshSpawner MeshSpawner::instance;

MeshSpawner::MeshSpawner()
{
	cam = Camera(Vector3(0, 0, -10));
}

MeshSpawner::~MeshSpawner()
{}

void MeshSpawner::Start()
{
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/MYawesomeface.png", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Configure the Frame Buffer object
	SetupPickingBuffer();

	shaders[0] = Shader("RenderTexture.shader");
	shaders[1] = Shader("RenderSingleColor.shader");
	shaders[2] = Shader("RenderColor_PerVertex.shader");

	meshes[0] = Cube();
	meshes[1] = ColoredCube();
	meshes[2] = Sphere();
	meshes[3] = Plane();

	shaders[0].Use();
	shaders[0].SetTexture("myTexture", 0);

	Transform t;
	t.position = position;
	t.rotation = rotation;
	t.scale = scale;
	t.color = color;
	t.meshToUse = meshSelection;
	t.shaderToUse = shaderSelection;

	transforms.push_back(t);

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	// When window is resized recalculate the projection matrix and also setup the Frame Buffer Object
	token = onWindowResize.subscribe([&](int width, int height, int leftPanel)
		{
			projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);
			SetupPickingBuffer();
		});
}

// FBO is the core of deferred rendering, SSAO, Shadow Mapping, Motion Blur, Post Processing and Bloom as it stores data into multiple textures in one pass and then combine them in the next pass
void MeshSpawner::SetupPickingBuffer()
{
	// A Framebuffer Object(FBO) is an offscreen render target
	// Normally OpenGL renders to default framebuffer which is the screen
	// FBO lets you render into a texture or render buffer instead
	// This is used for post proccessing, shadows, reflections, picking etc
	// The FBO is just a container - it holds attachments (textures/renderbuffers)
	// It has two attachment slots - color (what gets drawn) and depth (how deep each pixel is)
	glGenFramebuffers(1, &pickingFBO);

	// Binding FBO makes it as an active render target
	// All subsequent draw calls will render into the FBO, not onto the screen
	// It does not store the texture - it justs points to them 
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);

	// Color Texture - stores object IDs as colors
	// When we render into FBO, fragment shader output goes into this texture
	glGenTextures(1, &pickingTexture);
	glBindTexture(GL_TEXTURE_2D, pickingTexture);

	// Allocates an empty texture on the GPU
	// We want viewportData.width + viewportData.leftPanel because FBO doesn't know about the viewport offset and starts from 0,0 not from viewport.leftPanel,0
	// This makes the entire screen size texture we render to
	// If we just have viewportData.width then FBO starts from 0,0 on the left and ends on whatever the width is hence leaving the right side a size of viewportData.leftPanel not written to, hence we do not get exact points unless in the glReadPixel we offset out xPos by the width of left panel by subtracting it.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewportData.width + viewportData.leftPanel, viewportData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Texture Filtering
	// We need everything to be crisp that is why we use GL_NEAREST
	// If we interpolate then the color would mismatch hence giving us weird object IDs
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Attach the texture to FBO's color slot
	// Now when we render to FBO, pixel color would go into pickingTexture
	// 1 -> Which Framebuffer to attach to (currently bound one)
	// 2 -> Attachment slot: which slot in the FBO to attach to. There are multiple slots (8 in total) and can be used to store color, normal, position texture etc. Mostly used for deferred rendering, post processing etc.
	// 3 -> What kind of texture are we storing
	// 4 -> The actual texture object to attach
	// 5 -> Which mipmap level to render into
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingTexture, 0);

	// Depth buffer - Helps to identify which object is in front
	// For eg Object 0 is behing Object 1 but Object 0 is drawn after Object 1
	// In this case we will always pickup Object 0 although its behind.
	// Depth buffer won't render Object 0 as its behind
	// ==============================================
	// Renderbuffer is like a texture but has only write only operations
	// You cannot read a renderbuffer in a shader like we can read textures
	// It is faster than a texture for attachments because you only write to it which makes it ideal for depth buffer
	glGenRenderbuffers(1, &pickingDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, pickingDepth);

	// Allocate GPU memory for the render buffer
	// GL_DEPTH_COMPONENT tells OpenGL that this stores depth values not color values
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewportData.width + viewportData.leftPanel, viewportData.height);

	// Attach the render buffer to the FBO's depth slot
	// GL_DEPTH_COMPONENT tells the FBO this is the depth buffer not a color buffer
	// Now the FBO has both slots filled:
	//   GL_COLOR_ATTACHMENT0 -> pickingTexture  (stores ID colors)
	//   GL_DEPTH_ATTACHMENT  -> pickingDepth    (stores depth for correct overlap)
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickingDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Picking FBO not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Responsible for converting object ID to integer
	pickingShader = Shader("PickingShader.shader");
}

void MeshSpawner::Update()
{}

void MeshSpawner::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat3("Position", &position.x, 0.005f);
	ImGui::DragFloat3("Rotation", &rotation.x, 0.5f);
	ImGui::DragFloat3("Scale", &scale.x, 0.005f);

	ImGui::Columns(2, "shader_mesh_columns", false);

	// Column 1: Shader selection
	ImGui::Text("Shader");
	ImGui::RadioButton("Texture", &shaderSelection, 0);
	ImGui::RadioButton("Single Color", &shaderSelection, 1);
	ImGui::RadioButton("Vertex Color", &shaderSelection, 2);

	ImGui::NextColumn();

	// Column 2: Mesh selection
	ImGui::Text("Mesh");
	if (shaderSelection == 0 || shaderSelection == 1)
	{
		ImGui::RadioButton("Cube", &meshSelection, 0);
		ImGui::RadioButton("Sphere", &meshSelection, 2);
		ImGui::RadioButton("Plane", &meshSelection, 3);
	}
	else if (shaderSelection == 2)
	{
		ImGui::RadioButton("Colored Cube", &meshSelection, 1);
	}

	ImGui::Columns(1);

	if (shaderSelection == 1)
		ImGui::ColorEdit3("Color Pick", (float*)&color);

	if (ImGui::Button("Click to Add"))
	{
		Transform t;
		t.position = position;
		t.rotation = rotation;
		t.scale = scale;
		t.color = color;
		t.meshToUse = meshSelection;
		t.shaderToUse = shaderSelection;

		transforms.push_back(t);
	}

	ImGui::End();
}

void MeshSpawner::Render()
{
	RenderPickingPass(); // Before main render

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	view = cam.GetViewMatrix();

	Matrix4x4 model;

	for (int i = 0; i < transforms.size(); i++)
	{
		Transform t = transforms[i];

		model = Matrix4x4::Identity();

		model = Matrix4x4::Translation(model, t.position);
		model = Matrix4x4::Rotation(model, Vector3(1, 0, 0), t.rotation.x * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 1, 0), t.rotation.y * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 0, 1), t.rotation.z * (PI / 180));
		model = Matrix4x4::Scale(model, t.scale);

		shaders[t.shaderToUse].Use();
		shaders[t.shaderToUse].SetMat4_Custom("model", model.m);
		shaders[t.shaderToUse].SetMat4_Custom("view", view.m);
		shaders[t.shaderToUse].SetMat4_Custom("projection", projection.m);
		if (t.shaderToUse == 1)
		{
			shaders[t.shaderToUse].SetVec4("color", t.color);
		}

		meshes[t.meshToUse].Draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

// 1. glBindFramebuffer(pickingFBO)
//                         ↓
// 2. Render all objects with unique ID colors
//                         ↓
// 3. glBindFramebuffer(0)  — FBO now holds the ID texture in memory
//                         ↓
// 4. User clicks at screen xPos = 750, yPos = 400
//                         ↓
// 5. glReadPixels(750, 400, ...)  — read that pixel from FBO
//                         ↓
// 6. Decode RGBA → object ID
void MeshSpawner::RenderPickingPass()
{
	// Binding FBO makes it as an active render target
	// All subsequent draw calls will render into the FBO, not onto the screen
	// It does not store the texture - it justs points to them 
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white = no object (ID 255)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// This is a custom shader which takes the i value and colors the object accordingly in the fragment shader
	pickingShader.Use(); 
	pickingShader.SetMat4_Custom("view", view.m);
	pickingShader.SetMat4_Custom("projection", projection.m);

	for (int i = 0; i < transforms.size(); i++)
	{
		Transform t = transforms[i];

		Matrix4x4 model = Matrix4x4::Identity();
		model = Matrix4x4::Translation(model, t.position);
		model = Matrix4x4::Rotation(model, Vector3(1, 0, 0), t.rotation.x * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 1, 0), t.rotation.y * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 0, 1), t.rotation.z * (PI / 180));
		model = Matrix4x4::Scale(model, t.scale);

		pickingShader.SetMat4_Custom("model", model.m);
		// Setting up this uniform helps us to color the object
		pickingShader.SetInt("objectID", i);

		// Render Each thing onto the FBO
		meshes[t.meshToUse].Draw();
	}

	// Unbind the buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void MeshSpawner::HandleInput(GLFWwindow* window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		camMoveRotate = true;
	else
		camMoveRotate = false;

	if (camMoveRotate)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::RIGHT);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::UP);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::DOWN);
	}


	// This is where we get the ID when the player clicks on the screen
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !mKeyHeld)
	{
		mKeyHeld = true;
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		int id = GetObjectIDAtMouse((float)xPos, (float)yPos);

		if (id >= 0 && id < transforms.size())
		{
			std::cout << "Clicked object: " << id << std::endl;
			PopulateSelectedTransform(id);
		}
		else
		{
			std::cout << "Clicked Empty Space" << std::endl;
			currentSelectedTransform = nullptr;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		mKeyHeld = false;

}

// NOT USED
// Earilier was used to project a ray and get the object using the dot product
// If the dot product is within a certain threshold then select that obect
void MeshSpawner::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{
	if (camMoveRotate)
		cam.ProcessMouseMovement(xOffset, yOffset);

	float panelWidth = (float)viewportData.leftPanel;

	Vector4 mouseSome = Vector4(xPos, yPos, -10, 1);

	// if mouse is over left panel then don't process the world location and selection
	if (xPos < viewportData.leftPanel) return;

	Ray ray = ScreenToRay(xPos, yPos, view, projection);

	// Check Intersection
	// Core logic currently commented out because not being used
	int index = 0;
	//for (auto const &t : transforms)
	//{
	//	Vector3 toObject = (t.position - ray.origin).Normalize();
	//	float dot = ray.direction.Dot(toObject);

	//	std::cout << "Dot with object " << index << ": " << dot << std::endl;

	//	index++;
	//}
}

// Getting the Object using the FBO by reading the pixel when the mouse is clicked
int MeshSpawner::GetObjectIDAtMouse(float xPos, float yPos)
{
	// When you bind the Frame Buffer Object then glReadPixel reads from the framebuffer that is currently bound
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);

	// The viewport starts from top left (0,0) (y increases down) but in OpenGL/FBO texture coordinates start from bottom left (0,0) (y increases up)
	// This is why we flip the yPos we get.
	float flippedY = viewportData.height - yPos;

	// Contains the RGBA values
	// NOTE: GPU stores each channel of color values as 8 bit integer 0-255 and it is defined by GL_UNSIGNED_BYTE which is specified when we create a texture in glTexImage2D
	// So GPU automatically does this float -> byte = value * 255.0 -> round to nearest integer
	unsigned char pixel[4];
	// Copies the pixel's RGBA values into the into pixel[4] array from the FBO texture at (xPos, flippedY)
	glReadPixels(
		(int)xPos,          // x pixel coordinate to start reading from
		(int)flippedY,      // y pixel coordinate to start reading from
		1,                  // width: how many pixels to read horizontally
		1,                  // height: how many pixels to read vertically
		GL_RGBA,            // format: read all 4 channels
		GL_UNSIGNED_BYTE,   // type: each channel comes back as 0-255
		pixel               // destination: CPU memory to write into
	);

	// Print actual pixel values for Debugging
	std::cout << "Pixel RGBA: "
		<< (int)pixel[0] << ", "
		<< (int)pixel[1] << ", "
		<< (int)pixel[2] << ", "
		<< (int)pixel[3] << std::endl;

	// Unbind the buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// If all the values are 1 or white or 255 then return as its an empty space
	if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255 && pixel[3] == 255)
		return -1;

	// This the opposite of what we are doing in the shader
	// Each pixel has RGBA values in the form of bytes. We are using bit operators to reverse the encoding we have done
	// 
	// << (left shift operator) moves the bits to the left by appropriate ammount so that we get the correct data
	// 
	// | (OR operator) helps to add all the values together so that we can get the corract value back
	//
	// EXAMPLE
	// id = 305419896 
	// GPU stores it as R - 120, G - 86, B - 52, A - 18
	// glReadPixel populates pixel array as such pixel[0] - 120, pixel[1] - 86, pixel[2] - 52, pixel[3] - 18
	// We shift the bits to appropriate places using left bit shift operator(<<)
	// 
	//   // pixel[0] = 120 = byte 0, already in correct position
	//  pixel[0] = 00000000 00000000 00000000 01111000 = 120
	//	 // pixel[1] = 86 = byte 1, needs to move to bits 8-15
	//	pixel[1] << 8 = 00000000 00000000 01010110 00000000 = 22016
	//	 // pixel[2] = 52 = byte 2, needs to move to bits 16-23
	//	pixel[2] << 16 = 00000000 00110100 00000000 00000000 = 3407872
	//	 // pixel[3] = 18 = byte 3, needs to move to bits 24-31
	//	pixel[3] << 24 = 00010010 00000000 00000000 00000000 = 301989888
	// 
	// Now we use the OR operator to merge all the bytes and get the final value
	//
	// int id = pixel[0]         // 00000000 00000000 00000000 01111000
	//        | (pixel[1] << 8)  // 00000000 00000000 01010110 00000000
	//	      | (pixel[2] << 16) // 00000000 00110100 00000000 00000000
	//	      | (pixel[3] << 24);// 00010010 00000000 00000000 00000000
	//                           // ─────────────────────────────────────
	//                           // 00010010 00110100 01010110 01111000
	//								= 305419896
	// Hence we get the same value
	// --------------
	// NOTE: Here I am explicitly converting char datatype to int. This is not needed as C++ automatically does it when we perform any bitwise operation. It is called as integer promotion. But its always good to be explicit
	int id = (int)pixel[0] | ((int)pixel[1] << 8) | ((int)pixel[2] << 16) | ((int)pixel[3] << 24);
	return id;
}

void MeshSpawner::PopulateSelectedTransform(int selectedIndex)
{
	currentSelectedTransform = &transforms[selectedIndex];

	currentSelectedTransform->position.Print();
	currentSelectedTransform->rotation.Print();
}

void MeshSpawner::OnScroll(float xOffset, float yOffset)
{
	if (camMoveRotate)
		cam.ProcessMouseScroll(yOffset);
}

void MeshSpawner::Exit()
{
	glDeleteFramebuffers(1, &pickingFBO);
	glDeleteTextures(1, &pickingTexture);
	glDeleteRenderbuffers(1, &pickingDepth);

	glDisable(GL_DEPTH_TEST);

	for (Mesh& m : meshes)
	{
		m.CleanUp();
	}

	transforms.clear();

	scale = Vector3(1, 1, 1);

	onWindowResize.unsubscribe(token);
}

MeshSpawner* MeshSpawner::GetInstance()
{
	return &instance;
}


