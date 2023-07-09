#include "../include/Box.h"

float boxVertices[] = {
	 // positions        // normal          // color
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.98f, 0.89f, 0.32f,   // ���Ͻ��� (Back)
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.98f, 0.89f, 0.32f,   // ���½���
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.98f, 0.89f, 0.32f,   // ���½���
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.98f, 0.89f, 0.32f,   // ���Ͻ���

	-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.56f, 0.71f, 0.29f,   // ���Ͻ��� (Left)
	-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.56f, 0.71f, 0.29f,   // ���½���
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.56f, 0.71f, 0.29f,   // ���½���
	-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.56f, 0.71f, 0.29f,   // ���Ͻ���

	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.4f, 0.73f, 0.72f,   // ���Ͻ��� (Right)
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.4f, 0.73f, 0.72f,   // ���½���
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.4f, 0.73f, 0.72f,   // ���½���
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.4f, 0.73f, 0.72f,   // ���Ͻ���

	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���Ͻ��� (Bottom)
	 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���½���
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���½���
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���Ͻ���

	 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���Ͻ��� (Top)
	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���½���
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���½���
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.99f, 0.98f, 0.95f,   // ���Ͻ���
};

unsigned int boxIndices[] = {
	0, 1, 3,   // ���� (Back)
	1, 2, 3,

	4, 5, 7,   // ��� (Left)
	5, 6, 7,

	8, 9, 11,  // �ұ� (Right)
	9, 10, 11,

	12, 13, 15, // �ײ� (Bottom)
	13, 14, 15,

	16, 17, 19, // ���� (Top)
	17, 18, 19,
};

Box::Box()
{
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);
	glGenVertexArrays(1, &this->vao);
}

void Box::setData(float* vertices, unsigned int* indices, int numVertices, int numIndices)
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Box::setData()
{
	this->setData(boxVertices, boxIndices, 180, 30);
}

void Box::draw()
{
	this->shader.use();
	this->shader.setMatrix4("model", this->model);
	this->shader.setMatrix4("view", this->view);
	this->shader.setMatrix4("projection", this->projection);
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
