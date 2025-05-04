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

	shaderFile.open(shaderPath);
	std::stringstream vShaderStream, fShaderStream;

	// check for file errors
	if (shaderFile.fail())
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ SATYAM" << std::endl;
	}

	enum ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	ShaderType type = ShaderType::NONE;

	while (getline(shaderFile, line))
	{
		if (line.find("#Satyam") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
				continue;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
				continue;
			}
		}

		if (type == ShaderType::VERTEX)
		{
			vShaderStream << line << "\n";
		}
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
	//fShaderFile.close();

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

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(GetUniformLocation(name), (int)value); // Not Good for performance. We should cache the location
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value); // Not Good for performance. We should cache the location
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value); // Not Good for performance. We should cache the location
}

void Shader::SetVec2(const std::string& name, float x, float y)
{
	glUniform2f(GetUniformLocation(name), x, y);
}

// Responsible for caching the uniform location and returning it
int Shader::GetUniformLocation(const std::string& name)
{
	if(locationCache.find(name) != locationCache.end())
		return locationCache[name];

	int location = glGetUniformLocation(ID, name.c_str());
	if(location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	locationCache[name] = location;
	return location;
}
