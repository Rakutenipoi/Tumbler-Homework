#include "../include/Plane.h"

Plane::Plane()
{
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);
	glGenVertexArrays(1, &this->vao);
}

void Plane::setShader(Shader shader)
{
	this->shader = shader;
}

void Plane::setMatrix(MATRIX_TYPE type, glm::mat4 value)
{
	switch (type)
	{
		case MATRIX_TYPE::MODEL:
			this->model = value;
			break;
		case MATRIX_TYPE::VIEW:
			this->view = value;
			break;
		case MATRIX_TYPE::PROJECTION:
			this->projection = value;
			break;
		default:
			break;
	}
}

void Plane::setMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	this->setMatrix(MATRIX_TYPE::MODEL, model);
	this->setMatrix(MATRIX_TYPE::VIEW, view);
	this->setMatrix(MATRIX_TYPE::PROJECTION, projection);
}

void Plane::setData(float* vertices, unsigned int* indices, int numVertices, int numIndices)
{
	this->vertices = vertices;
	this->indices = indices;
	int sizeVertices = numVertices * sizeof(float);
	int sizeIndices = numIndices * sizeof(unsigned int);

	glBindVertexArray(this->vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, this->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, this->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Plane::draw(glm::vec3 color)
{
	this->shader.setVector3("color", color);
	this->shader.setMatrix4("model", this->model);
	this->shader.setMatrix4("view", this->view);
	this->shader.setMatrix4("projection", this->projection);
	this->shader.use();
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

glm::mat4 Plane::getMat(MATRIX_TYPE type)
{
	glm::mat4 ret = glm::mat4(1.0f);

	switch (type) {
		case MATRIX_TYPE::MODEL:
			ret = this->model;
			break;
		case MATRIX_TYPE::VIEW:
			ret = this->view;
			break;
		case MATRIX_TYPE::PROJECTION:
			ret = this->projection;
			break;
	}

	return ret;
}
