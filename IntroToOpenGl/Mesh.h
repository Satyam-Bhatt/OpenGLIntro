#pragma once
#include <vector>
#include <cstdint>
#include <glad.h>
#include "Vector.h"

using namespace Vector;

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Vector4 color;
};

class Mesh
{
public:
	std::vector<Vertex> vertices; // List of vertices
	std::vector<uint32_t> indices;

	uint32_t VAO, VBO, EBO;

	// Constructor
	Mesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices);

	void Bind() const;
	void Unbind() const;
	void Draw() const;
	void CleanUp() const;

	~Mesh();

private:
	void Setup();
};

