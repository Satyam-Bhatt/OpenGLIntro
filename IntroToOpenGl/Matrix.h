#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <iostream>
#include "Vector.h"

namespace Matrix
{
	struct Matrix2x2
	{
		float m[2][2];

		// Constructor
		Matrix2x2() {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (i == j) {
						m[i][j] = 1.0f;
					}
					else
						m[i][j] = 0.0f;
				}
			}
		}

		// Copy constructor
		Matrix2x2(const float(&matrix)[2][2]) {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					m[i][j] = matrix[i][j];
				}
			}
		}

		// Constructor from individual values
		Matrix2x2(float m00, float m01,
			float m10, float m11) {
			m[0][0] = m00; m[0][1] = m01;
			m[1][0] = m10; m[1][1] = m11;
		}

		// Element access
		float* operator[](int row) { return m[row]; }
		// For const objects
		const float* operator[](int row) const { return m[row]; }

		// Matrix Addition
		Matrix2x2 operator+(const Matrix2x2& other) const {
			Matrix2x2 result;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					result[i][j] = m[i][j] + other[i][j];
				}
			}
			return result;
		}

		// Matrix Subtraction
		Matrix2x2 operator-(const Matrix2x2& other) const {
			Matrix2x2 result;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					result[i][j] = m[i][j] - other[i][j];
				}
			}
			return result;
		}

		// Matrix Multiplication
		Matrix2x2 operator*(const Matrix2x2& other) const {
			Matrix2x2 result;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					result[i][j] = m[i][0] * other[0][j] + m[i][1] * other[1][j];
				}
			}
			return result;
		}

		// Vector Multiplication
		Vector::Vector2 operator*(const Vector::Vector2& vec) const
		{
			Vector::Vector2 result;
			result.x = m[0][0] * vec.x + m[0][1] * vec.y;
			result.y = m[1][0] * vec.x + m[1][1] * vec.y;
			return result;
		}

		// Scalar Multiplication
		Matrix2x2 operator*(float scalar) const {
			Matrix2x2 result;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					result[i][j] = m[i][j] * scalar;
				}
			}
			return result;
		}

		// Assignment operator
		Matrix2x2& operator=(const Matrix2x2& other) {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					m[i][j] = other[i][j];
				}
			}
			return *this;
		}

		Matrix2x2& operator*=(const Matrix2x2& other) {
			if (this != &other) {
				*this = *this * other;
			}
			return *this;
		}

		Matrix2x2& operator+= (const Matrix2x2& other) {
			*this = *this + other;
			return *this;
		}

		Matrix2x2& operator-= (const Matrix2x2& other) {
			*this = *this - other;
			return *this;
		}

		// Comparision Operator
		bool operator==(const Matrix2x2& other) const {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (m[i][j] != other[i][j]) {
						return false;
					}
				}
			}
			return true;
		}

		bool operator!=(const Matrix2x2& other) const {
			return !(*this == other);
		}

		bool isApproxEqual(const Matrix2x2& other, float epsilon = 1e-6f) const {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (std::abs(m[i][j] - other[i][j]) > epsilon) {
						return false;
					}
				}
			}
			return true;
		}

		bool isApproxNotEqual(const Matrix2x2& other, float epsilon = 1e-6f) const {
			return !isApproxEqual(other, epsilon);
		}

		Matrix2x2 Transpose() const {
			Matrix2x2 result;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					result[i][j] = m[j][i];
				}
			}
			return result;
		}

		void Print() const
		{
			for (int i = 0; i < 2; i++)
			{
				std::cout << "[";
				for (int j = 0; j < 2; j++) {
					std::cout << m[i][j];
					if (j < 1) std::cout << ", ";
				}
				std::cout << "]" << std::endl;
			}
		}

	};

	struct Matrix4x4
	{
		float m[4][4];

		// Constructor
		Matrix4x4() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == j) {
						m[i][j] = 1.0f;
					}
					else
						m[i][j] = 0.0f;
				}
			}
		}

		// Copy constructor
		Matrix4x4(const float(&matrix)[4][4]) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = matrix[i][j];
				}
			}
		}

		// Constructor from individual values
		Matrix4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) {
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		// Element access
		// Allows us to access the elements easily rather than accessing the member variables directly
		// Without this the code was
		// Matrix::Matrix4x4 mat;
		// mat.m[0][0] // To access element
		// With this we can do
		// Matrix::Matrix4x4 mat;
		// mat[0][0]
		// This works as we get the pointer to the row and then with the second [] we get the element
		// All these 4x4 matricies break down into float (*a) [4] discussed in MyMatrixClass.h
		float* operator[](int row) { return m[row]; }
		const float* operator[](int row) const { return m[row]; }

		// Matrix Addition
		Matrix4x4 operator+(const Matrix4x4& other) const {
			Matrix4x4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = m[i][j] + other.m[i][j];
				}
			}
			return result;
		}

		// Matrix Subtraction
		Matrix4x4 operator-(const Matrix4x4& other) const {
			Matrix4x4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = m[i][j] - other.m[i][j];
				}
			}
			return result;
		}

		// Matrix Multiplication
		Matrix4x4 operator*(const Matrix4x4& other) const {
			Matrix4x4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j] + m[i][3] * other.m[3][j];
				}
			}
			return result;
		}

		Vector::Vector3 operator*(const Vector::Vector3& vec) const {
			Vector::Vector3 result;
			result.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3] * 1.0f;
			result.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3] * 1.0f;
			result.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3] * 1.0f;

			return result;
		}

		// Compound assignment operators

		// this is the pointer to current object
		// in *this (*) just dereferences the pointer and returns the object
		Matrix4x4& operator*=(const Matrix4x4& other) {
			*this = *this * other;

			// return the refrence to current object
			return *this;
		}

		// const in the parameter means that the method doesn't modify the parameter when it is passed as refrence. 
		// If we don't use refrence then the method will copy the parameter which is not needed 
		// It is useless when the parameter is passed as value
		// When const is used before the pointer then 
		// void func(const Matrix4x4* ptr) {
		// 	  *ptr = something;     // ❌ Can't modify what ptr points to
		// 	  ptr = &other;         // ✅ Can change where ptr points
		// }

		// void func(Matrix4x4* const ptr) {
		// 	  *ptr = something;     // ✅ Can modify what ptr points to  
		// 	  ptr = &other;         // ❌ Can't change where ptr points
		// }

		// void func(const Matrix4x4* const ptr) {
		// 	  *ptr = something;     // ❌ Can't modify what it points to
		// 	  ptr = &other;         // ❌ Can't change where it points
		// }
		// const at the end of the method means that the method doesn't modify the object it is called on
		Matrix4x4& operator+=(const Matrix4x4& other) {
			*this = *this + other;
			return *this;
		}

		Matrix4x4& operator-=(const Matrix4x4& other) {
			*this = *this - other;
			return *this;
		}

		// Assignment operator
		Matrix4x4& operator=(const Matrix4x4& other) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = other.m[i][j];
				}
			}
			return *this;
		}

		// Comparison Operators
		bool operator==(const Matrix4x4& other) const {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (m[i][j] != other.m[i][j]) {
						return false;
					}
				}
			}
			return true;
		}

		bool operator!=(const Matrix4x4& other) const {
			return !(*this == other);
		}

		// Approximate equality comparison with default epsilon
		bool isApproxEqual(const Matrix4x4& other, float epsilon = 1e-6f) const {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (std::abs(m[i][j] - other.m[i][j]) > epsilon) {
						return false;
					}
				}
			}
			return true;
		}

		// Approximate inequality (for completeness)
		bool isApproxNotEqual(const Matrix4x4& other, float epsilon = 1e-6f) const {
			return !isApproxEqual(other, epsilon);
		}

		Matrix4x4 Transpose() const {
			Matrix4x4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = m[j][i];
				}
			}
			return result;
		}

		// Static methods
		static Matrix4x4 Scale(const Matrix4x4& matrix, Vector::Vector3 scale) {
			Matrix4x4 scaleMatrix;

			scaleMatrix[0][0] = scale.x;
			scaleMatrix[1][1] = scale.y;
			scaleMatrix[2][2] = scale.z;
			scaleMatrix[3][3] = 1.0f;

			return matrix * scaleMatrix;
		}

		static Matrix4x4 Translation(const Matrix4x4& matrix, Vector::Vector3 translation) {
			Matrix4x4 translationMatrix;

			translationMatrix[0][3] = translation.x;
			translationMatrix[1][3] = translation.y;
			translationMatrix[2][3] = translation.z;
			translationMatrix[3][3] = 1.0f;

			return matrix * translationMatrix;
		}


		static Matrix4x4 CreateRotationZ(float angle) {
			Matrix4x4 result;
			float c = cos(angle);
			float s = sin(angle);
			result[0][0] = c;  result[0][1] = -s;
			result[1][0] = s;  result[1][1] = c;
			return result;
		}

		static Matrix4x4 CreateRotationY(float angle) {
			Matrix4x4 result;
			float c = cos(angle);
			float s = sin(angle);
			result[0][0] = c;   result[0][2] = s;
			result[2][0] = -s;  result[2][2] = c;
			return result;
		}

		static Matrix4x4 CreateRotationX(float angle) {
			Matrix4x4 result;
			float c = cos(angle);
			float s = sin(angle);
			result[1][1] = c;  result[1][2] = -s;
			result[2][1] = s;  result[2][2] = c;
			return result;
		}

		static Matrix4x4 Rotation(const Matrix4x4& matrix, Vector::Vector3 axis, float rotationAngle) {
			Matrix4x4 rotationMatrix;

			if (axis.x != 0.0f) {
				rotationMatrix = rotationMatrix * CreateRotationX(rotationAngle);
			}
			if (axis.y != 0.0f) {
				rotationMatrix = rotationMatrix * CreateRotationY(rotationAngle);
			}
			if (axis.z != 0.0f) {
				rotationMatrix = rotationMatrix * CreateRotationZ(rotationAngle);
			}

			return matrix * rotationMatrix;
		}

		// Projection matrix in easier terms if we think of it like this
		// First scale the Square Frustrum to a cuboid
		// Then make the cuboid to cube and bring it to the origin or in easier terms multiply with the orthographic matrix after we get a cuboid
		// Assuming the camera is at the origin and right = -left, top = -bottom

		// Normalized z is inversely proportional to square of z projected because each component is divided by w component so if we divide z projected
		// with z projected we will get 1 hence loosing all the depth information. So we have to take the square of z projected to preserve the depth
		// inforamtion. 
		// But this causes a problem. Because the values are now z^2 the curve is not linear. So we get hight precision near the near plane and
		// low precision in the far plane as values are pretty close in the far plane.
		static Matrix4x4 CreateProjectionMatrix_FOV(float angle, float width, float height, float near, float far)
		{
			float aspectRatio = width / height;
			float top = tan(angle / 2) * near;
			float right = aspectRatio * top;

			Matrix4x4 result;

			result[0][0] = near / right;

			result[1][1] = near / top;

			result[2][2] = -(far + near) / (far - near);
			result[2][3] = (-2 * far * near) / (far - near);

			result[3][2] = -1;

			return result;
		}

		// Debug print function
		void Print() const {
			for (int i = 0; i < 4; i++) {
				std::cout << "[";
				for (int j = 0; j < 4; j++) {
					std::cout << m[i][j];
					if (j < 3) std::cout << ", ";
				}
				std::cout << "]" << std::endl;
			}
		}
	};
}

#endif
