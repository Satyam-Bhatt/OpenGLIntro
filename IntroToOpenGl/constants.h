#include <glad.h>
#include <GLFW/glfw3.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PI 3.14159265358979323846

// constexpr is like a #define but for compile-time constants and also is type-safe
constexpr int VIEWPORT = 75;

struct ViewportData {
    int width;
    int height;
};

struct MouseValues{
    float lastMouseX;
    float lastMouseY;
    float mouseXOffset;
    float mouseYOffset;
    bool firstMouse;
};

// Don't initialize extern variables in a header file
extern ViewportData viewportData;
extern MouseValues mouseValues;
extern GLFWwindow* window;
extern float deltaTime;

#endif