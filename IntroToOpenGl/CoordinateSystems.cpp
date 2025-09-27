#include "CoordinateSystems.h"

CoordinateSystems CoordinateSystems::instance;

CoordinateSystems::CoordinateSystems()
{
}

CoordinateSystems::~CoordinateSystems()
{
	Exit();
}

void CoordinateSystems::Start()
{
	shader = Shader("CoordinateSystems.shader");

	unsigned int indices[] =
	{
		// Front face
		0, 1, 2,
		1, 3, 2,
		// Back face  
		4, 6, 5,
		5, 6, 7,
		// Left face
		0, 2, 4,
		2, 6, 4,
		// Right face
		1, 5, 3,
		3, 5, 7,
		// Top face
		2, 3, 6,
		3, 7, 6,
		// Bottom face
		0, 4, 1,
		1, 4, 5
	};

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f,
		 0.5f, -0.5f, 0.5f, 1.0f,
		-0.5f,  0.5f, 0.5f, 1.0f,
		 0.5f,  0.5f, 0.5f, 1.0f
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CoordinateSystems::Update()
{
}

void CoordinateSystems::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("X axis", &rotX);
	ImGui::Checkbox("Y axis", &rotY);
	ImGui::Checkbox("Z axis", &rotZ);

	ImGui::End();
}

void CoordinateSystems::Render()
{

	float axisX = rotX?1:0;
	float axisY = rotY?1:0;
	float axisZ = rotZ?1:0;

	Matrix::Matrix4x4 rotMat;
	rotMat = Matrix::Matrix4x4::Rotation(rotMat, Vector::Vector3(axisX, axisY, axisZ), glfwGetTime());

	shader.Use();
	shader.SetMat4_Custom("rot", rotMat.m);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CoordinateSystems::Exit()
{
	if(VAO != 0) glDeleteVertexArrays(1, &VAO);
	if(VBO != 0) glDeleteBuffers(1, &VBO);
	if(EBO != 0) glDeleteBuffers(1, &EBO);
	if(shader.ID != 0) glDeleteProgram(shader.ID);

}

CoordinateSystems* CoordinateSystems::GetInstance()
{
	return &instance;
}

Matrix4x4 CoordinateSystems::CreateModelMatrix()
{
	return Matrix4x4();
}

Matrix4x4 CoordinateSystems::CreateViewMatrix()
{
	return Matrix4x4();
}

// Projection matrix in easier terms if we think of it like this
// First scale the Square Frustrum to a cuboid
// Then make the cuboid to cube and bring it to the origin or in easier terms multiply with the orthographic matrix after we get a cuboid
// Assuming the camera is at the origin and right = -left, top = -bottom

// Normalized z is inversely proportional to square of z projected because each component is divided by w component so if we divide z projected
// with z projected we will get 1 hence loosing all the depth information. So we have to take the square of z projected to preserve the depth
// inforamtion. 
// But this causes a problem. Because the values are now z^2 the curve is not linear. So we get hight precision near the near plane and
// low precision in the far plane as values are pretty close in the far plane.
Matrix4x4 CoordinateSystems::CreateProjectionMatrix_FOV(float angle, float width, float height, float near, float far)
{
	float aspectRatio = width / height;
	float top = tan(angle / 2) * near;
	float right = aspectRatio * top;

	Matrix4x4 tempMatrix;

	tempMatrix[0][0] = near / right;
	tempMatrix[0][1] = 0;
	tempMatrix[0][2] = 0;
	tempMatrix[0][3] = 0;

	tempMatrix[1][0] = 0;
	tempMatrix[1][1] = near / top;
	tempMatrix[1][2] = 0;
	tempMatrix[1][3] = 0;

	tempMatrix[2][0] = 0;
	tempMatrix[2][1] = 0;
	tempMatrix[2][2] = -(far + near) / (far - near);
	tempMatrix[2][3] = (-2 * far * near) / (far - near);

	tempMatrix[3][0] = 0;
	tempMatrix[3][1] = 0;
	tempMatrix[3][2] = -1;
	tempMatrix[3][3] = 0;

	return tempMatrix;
}

Matrix4x4 CoordinateSystems::CreateProjectionMatrix_RAW(float right, float left, float bottom, float top, float near, float far)
{
	Matrix4x4 tempMatrix;

	tempMatrix[0][0] = 2 * near / (right - left);
	tempMatrix[0][1] = 0;
	tempMatrix[0][2] = (right + left) / (right - left);
	tempMatrix[0][3] = 0;

	tempMatrix[1][0] = 0;
	tempMatrix[1][1] = 2 * near / (top - bottom);
	tempMatrix[1][2] = (top + bottom) / (top - bottom);
	tempMatrix[1][3] = 0;

	tempMatrix[2][0] = 0;
	tempMatrix[2][1] = 0;
	tempMatrix[2][2] = -(far + near) / (far - near);
	tempMatrix[2][3] = (- 2 * far * near) / (far - near);

	tempMatrix[3][0] = 0;
	tempMatrix[3][1] = 0;
	tempMatrix[3][2] = -1;
	tempMatrix[3][3] = 0;

	return tempMatrix;
}

Matrix4x4 CoordinateSystems::CreateProjectionMatrix_ORTHO(float left, float right, float bottom, float top, float far, float near)
{
	Matrix4x4 tempMatrix;

	tempMatrix[0][0] = 2 / (right - left);
	tempMatrix[0][1] = 0;
	tempMatrix[0][2] = 0;
	tempMatrix[0][3] = -(right + left) / (right - left);

	tempMatrix[1][0] = 0;
	tempMatrix[1][1] = 2 / (top - bottom);
	tempMatrix[1][2] = 0;
	tempMatrix[1][3] = -(top + bottom) / (top - bottom);

	tempMatrix[2][0] = 0;
	tempMatrix[2][1] = 0;
	tempMatrix[2][2] = -2 / (far - near);
	tempMatrix[2][3] = -(far + near) / (far - near);

	tempMatrix[3][0] = 0;
	tempMatrix[3][1] = 0;
	tempMatrix[3][2] = 0;
	tempMatrix[3][3] = 1;

	return tempMatrix;
}

Matrix4x4 CoordinateSystems::CreateInfinitePerspectiveMatrix(float, float right, float top)
{
	return Matrix4x4();
}


