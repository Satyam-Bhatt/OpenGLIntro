#ifndef CAMERA_H
#define CAMERA_H

#include <glad.h>
#include <glm.hpp>
#include "Vector.h"
#include "Matrix.h"

using namespace Matrix;
using namespace Vector;

// Defines several possible options for camera movement.
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
// By default the camera points to the positive X axis and we need to rotate it by 90 degrees so that it points towards positive Z axis
const float YAW = 90,
const float PITCH = 0;
const float SPEED = 5;
const float SENSTIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	Vector3 CameraPosition;
};

#endif
