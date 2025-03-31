#pragma once
#include <stdio.h>
#include <iostream>
#include <GLFW/glfw3.h>


class GameState
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(GLFWwindow* window) = 0;
	virtual void Exit() = 0;
};