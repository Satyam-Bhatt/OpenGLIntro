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
	};
}

#endif
