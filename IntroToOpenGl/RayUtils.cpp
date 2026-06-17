#include "RayUtils.h"
#include "constants.h"

Ray RayUtils::ScreenToRay(float xPos, float yPos, Matrix::Matrix4x4 view, Matrix::Matrix4x4 projection)
{
	// (xPos - viewportData.leftPanel) - To offset the starting point so that 0 is where the left panel ends
	// / viewportData.width - To make the range from 0 - 1 (normalization)
	// * 2.0f - 1.0f - To make the values go from -1 to 1
	float ndcX = (xPos - viewportData.leftPanel) / viewportData.width * 2.0f - 1.0f;
	// first normalize by dividing by viewposrData.width and then make the range from -1 to 1 by *2.0f - 1.0f
	// negate it to flip the coordinate space as in screenspace the y is 0 at the top and increases as we go down
	float ndcY = -(yPos / viewportData.width * 2.0f - 1.0f);

	// OpenGL CVV is from -1 to 1 so the Z values are used as such
	Vector::Vector4 nearPoint = Vector::Vector4(ndcX, ndcY, -1, 1); // Clip Space
	Vector::Vector4 farPoint = Vector::Vector4(ndcX, ndcY, 1, 1); // Clip Space

	// Clip to world
	// Suppose projection * view is M
	// clipPoint = M * worldPoint;
	// M⁻¹ * clip = M⁻¹ * M * world
	// M⁻¹* clip = I * world
	// M⁻¹ * clip = world
	// Hence  world = M⁻¹ * clip
	// M⁻¹ First undoes the projection — expands the cube back into a frustum. Then undoes the view — moves everything back to where the camera actually is in the world
	Matrix::Matrix4x4 inverseVP = (projection * view).Inverse();

	// Convert clip to world space
	Vector::Vector4 worldNear = inverseVP * nearPoint;
	Vector::Vector4 worldFar = inverseVP * farPoint;

	// Perspective divide
	// This converts homogenous to cartesian coordinates
	// Divide by w so that the w component comes back to be 1
	// W is the bottom most row of the matrix 4x4
	// W scales each axis by the depth value so its necessary to divide by W to get the world values unaffected by the depth values
	// TODO: Sister Times
	worldNear /= worldNear.w;
	worldFar /= worldFar.w;

	// We need to return a ray so that we can check the dot product hence computing if it is over the object or not
	Ray ray;

	ray.origin = Vector::Vector3(worldNear.x, worldNear.y, worldNear.z);
	ray.direction = Vector::Vector3(worldFar - worldNear).Normalize();

	return ray;
}
