#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<uint32_t> _indices)
{
	// This makes the copy of the vertices
	//vertices = _vertices;
	//indices = _indices;

	// std::move just hands over the memory and _vertice and _indices become empty after this
	vertices = std::move(_vertices);
	indices = std::move(_indices);

	Setup();
}

void Mesh::Bind() const
{
	glBindVertexArray(VAO);
}

void Mesh::Unbind() const
{
	glBindVertexArray(0);
}

void Mesh::Draw() const
{
	Bind();
	// To draw triangles from indices/index buffer defined in EBO we use glDrawElements
	// arg1 -> Type of primitive we want to draw
	// arg2 -> number of elements we want to draw. A square is made up of 2 triangles hence 6 vertices
	// arg3 -> The type of the indicies in the EBO
	// arg4 -> specify an offset in the EBO (or pass in an index array, but that is when you're not using element buffer objects)
	//glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	Unbind();
}

void Mesh::CleanUp() const
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// Bind VAO, VBO, EBO and also relevant attributes as per vertex struct
void Mesh::Setup()
{}
