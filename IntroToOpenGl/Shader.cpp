#include "Shader.h"

// Shader Class

Shader::Shader()
{
	ID = 0;
}

Shader::Shader(const std::string& shaderPath)
{
	// 1. retrieve the vertex/fragment source code from file path
	std::string vertexrCode; // Variable that stores the vertex shader code
	std::string fragmentCode; // Variable that stores the fragment shader code

	// This is an input file stream class that allows to read data from files. It is used for file input operations.
	std::ifstream shaderFile;// Input a file stream for vertex shader

	// To open a file and read from it
	shaderFile.open(shaderPath);
	std::stringstream vShaderStream, fShaderStream;

	// check for file errors
	if (shaderFile.fail())
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ SATYAM" << std::endl;
	}

	// Different shader type that are contained in a shader file. We write to each stream depending on the type
	enum ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	// Line which we will write to each line from the shader file
	std::string line;
	ShaderType type = ShaderType::NONE;

	// Getting each line then checking if it contains #Satyam vertex or #Satyam fragment and then adding it to the stream as per the ShaderType
	while (getline(shaderFile, line))
	{
		// if the line contains #Satyam
		if (line.find("#Satyam") != std::string::npos)
		{
			// if the line contains vertex then set the type to vertex
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
				continue;
			}
			// if the line contains fragment then set the type to fragment
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
				continue;
			}
		}

		// if the type is vertex then add the line to the vertex stream
		if (type == ShaderType::VERTEX)
		{
			vShaderStream << line << "\n";
		}
		// if the type is fragment then add the line to the fragment stream
		else if (type == ShaderType::FRAGMENT)
		{
			fShaderStream << line << "\n";
		}
	}

	// read file's buffer contents into streams || Claude Define || 
	// When ifstream object is created it is in the memory but the content are on the disk. rdBuf() just gets the pointer to the 
	// buffer/stream object created. It does not get the contents. 
	// Below we get the pointer to that object and then the stream operator << is used to get the contents into the stream
	//vShaderStream << vShaderFile.rdbuf(); // NOT USING THEM AS WE ARE CREATING ONE SHADER FILE 
	//fShaderStream << fShaderFile.rdbuf(); // NOT USING THEM AS WE ARE CREATING ONE SHADER FILE

	// close file handlers
	shaderFile.close();

	// convert stream into string
	vertexrCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	const char* vShaderCode = vertexrCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// printing linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(ID);
}

// Set bool uniform. It checks if we have the location cached or not
void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(GetUniformLocation(name), (int)value);
}

// Set int uniform
void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

// Set float uniform
void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

// Set vec2 uniform
void Shader::SetVec2(const std::string& name, float x, float y)
{
	glUniform2f(GetUniformLocation(name), x, y);
}

// Set texture uniform
void Shader::SetTexture(const std::string& name, int slot)
{
	// To set multiple textures in a fragment shader we set the location value to the texture sampler.
	// This location is known as a texture unit
	// This is used to assign texture units to samplers that are defined as uniform variables
	glUniform1i(GetUniformLocation(name), slot);
}

void Shader::SetMat4(const std::string& name, glm::mat4 matrix)
{
	// First parameter is the location of the uniform variable
	// Second parameter is the number of matrices to be passed
	// Third parameter is whether the matrices are transposed or not. OpenGl expects the matrices to be in column major form default in GLM
	// Fourth parameter is the matrix data. As the glm matrix does not match the opengl matrix we have to use glm::value_ptr
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetMat4_Custom(const std::string& name, float(&a)[4][4])
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, a);
}

// Responsible for caching the uniform location and returning it
int Shader::GetUniformLocation(const std::string& name)
{
	// If the name is in the unordered map then return the location which is int
	if(locationCache.find(name) != locationCache.end())
		return locationCache[name];

	// if the location is not in the map then get it from the GPU
	int location = glGetUniformLocation(ID, name.c_str());
	// If the location is -1 then the uniform variable doesn't exist and we print a warning
	if(location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	// If the location is not -1 then add it to the map with key as name and return the location
	locationCache[name] = location;
	return location;
}
