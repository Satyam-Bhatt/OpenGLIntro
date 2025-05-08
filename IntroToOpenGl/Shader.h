#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

// If this is not defined, then this is the first include and the code below will be compiled but if this is already defined then 
// the code below will not be compiled. Using these little lines of code informs your compiler to only include and compile this header file 
// if it hasn't been included yet, even if multiple files include the shader header. This prevents linking conflicts.
#ifndef SHADERCLASS_H 
#define SHADERCLASS_H

class Shader
{
public:
    // The Program ID
    unsigned int ID;

    // Default constructor. It should be there so that we can instantiate this class
	Shader();

    // Constructor reads and builds the shader
    Shader(const std::string& shaderPath);
    
    // Use/activate the shader
    void Use();

    // Utility uniform functions
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, float x, float y);

private:

    // Store the location of the uniforms
    std::unordered_map <std::string, int> locationCache;

    int GetUniformLocation(const std::string& name);


};

#endif 

