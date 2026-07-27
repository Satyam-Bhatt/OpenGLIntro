#ifndef UTILS_H
#define UTILS_H

#include "Vector.h"
#include "Matrix.h"

namespace Utils
{
	bool WorldToScreen(const Vector::Vector3& worldPos, const Matrix::Matrix4x4& view, const Matrix::Matrix4x4 projection, Vector::Vector2& outScreenXY);
}

#endif


