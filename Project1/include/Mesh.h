#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.h"

struct Vertex {
	// 顶点位置
	glm::vec3 position;
	// 顶点法线
	glm::vec3 normal;
	// 顶点纹理坐标
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh
{
public:
	// 网格数据
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// 函数
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader shader);

protected:
	unsigned int vao, vbo, ebo;
	void setupMesh();

};

