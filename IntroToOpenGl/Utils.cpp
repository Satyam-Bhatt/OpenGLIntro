#include "Utils.h"

// Convert World to Screen points
bool Utils::WorldToScreen(const Vector::Vector3& worldPos, const Matrix::Matrix4x4& view, const Matrix::Matrix4x4 projection, Vector::Vector2& outScreenXY)
{
	Matrix::Matrix4x4 viewProjection = projection * view;

	Vector::Vector4 clip = viewProjection * Vector::Vector4(worldPos.x, worldPos.y, worldPos.z, 1.0f);

	// Point behind camera
	if (clip.w <= 0.0f)
		return false;

	// Perspective divide: clip space -> NDC
	float ndcX = clip.x / clip.w;
	float ndcY = clip.y / clip.w;

	// ndcX * 0.5f + 0.5f -> Converts the ndcX from -1-1 to 0-1
	// * viewportData.width -> Multiply with normalized ndcX to get the value in the viewport. 0 is left || 0.5 is in middle(half of viewport width) || 1 is on the right
	// viewportData.leftPanel + -> Adding the offset
	outScreenXY.x = viewportData.leftPanel + (ndcX * 0.5f + 0.5f) * viewportData.width;
	// Similar to what is happening above
	// Inverse the Y coordinates as Screen Space increases downwards
	outScreenXY.y = (1.0f - (ndcY * 0.5f + 0.5f)) * viewportData.height;

	return true;
}
