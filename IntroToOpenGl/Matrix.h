#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <iostream>

namespace Matrix
{
	struct Matrix4x4
	{
		float m[4][4] = { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} };

		// Constructor
		Matrix4x4() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == j) {
						m[i][j] = 1.0f;
					}
					else
						m[i][j] = 0.0f;
					std::cout << m[i][j] << " ";
				}
				std::cout << std::endl;
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

		// Compound assignment operators

		// this is the pointer to current object
		// in *this (*) just dereferences the pointer and returns the object
		Matrix4x4& operator*=(const Matrix4x4& other) {
			*this = *this * other;

			// return the refrence to current object
			return *this;
		}

		Matrix4x4& operator+=(const Matrix4x4& other) {
			*this = *this + other;
			return *this;
		}
	};
}

#endif
