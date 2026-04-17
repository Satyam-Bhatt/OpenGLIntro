#include "constants.h"

// This viewport represents the inner area where we render our scene.
ViewportData viewportData = { 0, 0 };
MouseValues mouseValues = { 400,300, 0, 0, true };
GLFWwindow* window = nullptr;
float deltaTime = 0.0f;