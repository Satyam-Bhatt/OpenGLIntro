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
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// arg 2 -> vertices.size() gives the number of elements and sizeof(Vertex) gives the size of each element in bytes. Multiply to get the entire size in bytes
	// sizeof(veritces) give us the size of std::vector which is 24 bytes
	// Roughly what std::vector looks like under the hood
	// struct vector {
	// 	  Vertex* data;     // 8 bytes — pointer to heap allocated array
	// 	  size_t   size;     // 8 bytes — number of elements
	// 	  size_t   capacity; // 8 bytes — how much space is allocated
	// };
	// total = 24 bytes, always
	// 
	// arg 3 -> glBufferData is a C function and expects raw pointer not a C++ object
	// vertices is a std::vector - a C++ object and openGL does not know what std::vector is
	// vertices.data() is a raw pointer to the first element giving Vertex*
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	// These are fragile in the case of a struct as we assume that compiler packs them with 0 padding
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	// glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));

	// location 0 — position (vec3)
	// arg 6 -> offsetof is a macro which looks like this --> #define offsetof(Type, field) (size_t)&(((Type*)0)->field)
	// (Type*)0 hypothetical places the struct at memory address 0 and then find out the address of a particular field. This allows to figure out the offsets in memoory of each field in a struct
	// If field is uv then (Vertex*)0)->uv would return 24 as uv sits at 24 byte (0-11(position) || 12-23(normal) || 24-31(uv) || 32-47(color))
	// &(...) gives the address as 24 as the address starts at 0
	// (size_t) casts the pointer to a number
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// location 1 — normal (vec3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// location 2 — uv (vec2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);

	// location 3 — color (vec4)
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}
