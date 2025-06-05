#include "MipmapOptions.h"

MipmapOptions MipmapOptions::instance;

MipmapOptions::MipmapOptions()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	texture = 0;
}

MipmapOptions::~MipmapOptions()
{
	Exit();
}

void MipmapOptions::Start()
{
	shader = Shader("MipmapOptions.shader");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/awesomeface.png", &width, &height, &nChannels, 0);

	if (data)
	{
		// RGBA as we also have alpha channel because of PNG format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}

	stbi_image_free(data);

	// Rectangle
	float vertices[] =
	{
		// Positions       // Colors          // TexCoords
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left 
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};

	int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.Use();
	shader.SetTexture("mipMapTexture", 0);
}

void MipmapOptions::Update()
{
	// When the scale multiplier changes, update the vertices of the rectangle hence updating the VBO
	if (currentScaleMultiplier != scaleMultiplier || !TexCoordMatchCheck())
	{
		// Scaling Matrix
		float matrix3[3][3] = {
		{ scaleMultiplier,		0.0f	  ,		 0.0f		},
		{		0.0f	 , scaleMultiplier,	     0.0f	    },
		{		0.0f     ,		0.0f	  , scaleMultiplier } };

		float vertices[] =
		{
			  // Positions       // Colors          // TexCoords
			  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, texCoords[0][0], texCoords[0][1], // Top Right
			  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, texCoords[1][0], texCoords[1][1], // Bottom Right
			 -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, texCoords[2][0], texCoords[2][1], // Bottom Left
			 -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, texCoords[3][0], texCoords[3][1]  // Top Left
		};

		// Matrix Multiplication so that the vertices are scaled
		// Clever way of getting the number of elements in an array -> (sizeof(vertices) / sizeof(vertices[0]))
		for (int i = 0; i < (sizeof(vertices) / sizeof(vertices[0])); i = i + 8)
		{
			float x = vertices[i];
			float y = vertices[i + 1];
			float z = vertices[i + 2];

			for (int j = 0; j < 3; j++)
			{
				vertices[i + j] = matrix3[j][0] * x + matrix3[j][1] * y + matrix3[j][2] * z;
			}
		}

		// We don't ned to update the VAO as it just stores the configuration and the configration like the color and all is the same.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		currentScaleMultiplier = scaleMultiplier;
	}

	// Texture Filtering
	// If there is any change in the filtering we change the filtering of the texture. We don't need to update the Wrapping
	if (currentMinFilterIndex != minFilterIndex || currentMagFilterIndex != magFilterIndex)
	{
		glBindTexture(GL_TEXTURE_2D, texture);

		if (minFilterIndex == 0) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else if (minFilterIndex == 1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		else if (minFilterIndex == 2) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else if (minFilterIndex == 3) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else if (minFilterIndex == 4) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else if (minFilterIndex == 5) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (magFilterIndex == 0)  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		else if (magFilterIndex == 1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		currentMinFilterIndex = minFilterIndex;
		currentMagFilterIndex = magFilterIndex;

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Texture Wrapping
	// When we make a texture smaller/bigger by changing texture coordinates it still uses the filtering technique that we have set.
	if (currentWrapSIndex != wrapSIndex || currentWrapTIndex != wrapTIndex || currentBorderColor != borderColor)
	{
		glBindTexture(GL_TEXTURE_2D, texture);

		if (wrapSIndex == 0) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		else if (wrapSIndex == 1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		else if (wrapSIndex == 2) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		else if (wrapSIndex == 3)
		{
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		}

		if (wrapTIndex == 0) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		else if (wrapTIndex == 1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		else if (wrapTIndex == 2) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		else if (wrapTIndex == 3)
		{
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}

		currentWrapSIndex = wrapSIndex;
		currentWrapTIndex = wrapTIndex;
		for(int i = 0; i < 4; i++) currentBorderColor[i] = borderColor[i];

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void MipmapOptions::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);
	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	// Create two-column layout
	if (ImGui::BeginTable("TextureSettings", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_PadOuterX))
	{
		// Column headers
		ImGui::TableSetupColumn("Filtering", ImGuiTableColumnFlags_WidthFixed, 280.0f);
		ImGui::TableSetupColumn("Wrapping", ImGuiTableColumnFlags_WidthFixed, 400.0f);
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();

		// Left column - Filtering techniques and Scale
		ImGui::TableSetColumnIndex(0);

		ImGui::DragFloat("Scale", &scaleMultiplier, 0.005f, 0.0f);

		ImGui::PushItemWidth(200.0f);
		ImGui::Combo("##MinFilter", &minFilterIndex, TEXTURE_MIN_FILTERS, IM_ARRAYSIZE(TEXTURE_MIN_FILTERS));
		ImGui::SameLine();
		ImGui::TextDisabled("Min (?)");
		if (ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("When the texture is minified or scaled down.");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::Combo("##MagFilter", &magFilterIndex, TEXTURE_MAG_FILTERS, IM_ARRAYSIZE(TEXTURE_MAG_FILTERS));
		ImGui::SameLine();
		ImGui::TextDisabled("Mag (?)");
		if (ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("When the texture is magnified or scaled up.");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::PopItemWidth();

		// Right column - Wrapping techniques and Texture Coordinates
		ImGui::TableSetColumnIndex(1);

		ImGui::Combo("##WrapS", &wrapSIndex, TEXTURE_WRAP_S, IM_ARRAYSIZE(TEXTURE_WRAP_S));
		ImGui::SameLine();
		ImGui::TextDisabled("S-Axis (?)");
		if (ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Only works when the texture coordinates are changed. Responsible for X axis");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		ImGui::Combo("##WrapT", &wrapTIndex, TEXTURE_WRAP_T, IM_ARRAYSIZE(TEXTURE_WRAP_T));
		ImGui::SameLine();
		ImGui::TextDisabled("T-Axis (?)");
		if (ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Only works when the texture coordinates are changed. Responsible for Y axis");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}

		// Border color (only show when needed, in wrapping column)
		if (wrapSIndex == 3 || wrapTIndex == 3)
			ImGui::ColorEdit3("Border Color", &borderColor[0]);

		ImGui::Text("Texture Coordinates");

		// Texture coordinates in a 2x2 grid layout
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat2("Top Right", &texCoords[0][0], 0.005f);
		ImGui::SameLine();
		ImGui::DragFloat2("Top Left", &texCoords[3][0], 0.005f);

		ImGui::DragFloat2("Bottom Right", &texCoords[1][0], 0.005f);
		ImGui::SameLine();
		ImGui::DragFloat2("Bottom Left", &texCoords[2][0], 0.005f);
		ImGui::PopItemWidth();

		ImGui::EndTable();
	}

	ImGui::End();
}

void MipmapOptions::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MipmapOptions::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	if (texture != 0) glDeleteTextures(1, &texture);
	glDeleteProgram(shader.ID);
}

MipmapOptions* MipmapOptions::GetInstance()
{
	return &instance;
}

bool MipmapOptions::TexCoordMatchCheck()
{
	if (currentTexCoords[0][0] != texCoords[0][0]) return false;
	if (currentTexCoords[0][1] != texCoords[0][1]) return false;
	if (currentTexCoords[1][0] != texCoords[1][0]) return false;
	if (currentTexCoords[1][1] != texCoords[1][1]) return false;
	if (currentTexCoords[2][0] != texCoords[2][0]) return false;
	if (currentTexCoords[2][1] != texCoords[2][1]) return false;
	if (currentTexCoords[3][0] != texCoords[3][0]) return false;
	if (currentTexCoords[3][1] != texCoords[3][1]) return false;

	return true;
}
