#ifndef RAY_H
#define RAY_H

#include "Vector.h"
#include "Matrix.h"

struct Ray
{
	Vector::Vector3 origin;
	Vector::Vector3 direction;
};

namespace RayUtils
{
	Ray ScreenToRay(float xPos, float yPos, Matrix::Matrix4x4 view, Matrix::Matrix4x4 projection);
}

#endif
