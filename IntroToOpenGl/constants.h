#include <glad.h>
#include <GLFW/glfw3.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

// constexpr is like a #define but for compile-time constants and also is type-safe
constexpr int VIEWPORT = 75;

struct ViewportData {
    int width;
    int height;
};

// Don't initialize extern variables in a header file
extern ViewportData viewportData;
extern GLFWwindow* window;
extern float deltaTime;

#endif