#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"

struct Vertex {
	// ����λ��
	glm::vec3 position;
	// ���㷨��
	glm::vec3 normal;
	// ������������
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh
{
public:
	// ��������
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// ����
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader shader);

protected:
	unsigned int vao, vbo, ebo;
	void setupMesh();

};

