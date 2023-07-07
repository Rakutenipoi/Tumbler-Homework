#include "..\include\Sphere.h"

#define M_PI 3.1415926

Sphere::Sphere(float radius, int slices, int stacks)
{
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;
    this->setColor(glm::vec3(1.0f));
    this->createSphere(radius, slices, stacks);
	
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::setColor(glm::vec3 color)
{
    this->color = color;
}

void Sphere::draw()
{
	this->shader.use();
    this->shader.setVector3("color", color);
	this->shader.setMatrix4("model", this->model);
	this->shader.setMatrix4("view", this->view);
	this->shader.setMatrix4("projection", this->projection);
	glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->slices * this->stacks * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::createSphere(float radius, int slices, int stacks)
{
    // 创建球面顶点(共有(slices + 1) * (stacks + 1)个顶点)
    for (int y = 0; y <= stacks; y++) {
        float alpha = (float)y / (float)stacks * M_PI;
        for (int x = 0; x <= slices; x++) {
            float beta = (float)x / (float)slices * 2 * M_PI;
            float xPos = sin(alpha) * cos(beta);
            float yPos = cos(alpha);
            float zPos = sin(alpha) * sin(beta);

            this->vertices.push_back(xPos * radius);
            this->vertices.push_back(yPos * radius);
            this->vertices.push_back(zPos * radius);

            this->normals.push_back(xPos);
            this->normals.push_back(yPos);
            this->normals.push_back(zPos);
        }
    }

    // 构建三角形图元
    for (int y = 0; y <= stacks; y++) {
        for (int x = 0; x <= slices; x++) {
            this->indices.push_back(y * (slices + 1) + x); // 左上
            this->indices.push_back((y + 1) * (slices + 1) + x); // 左下
            this->indices.push_back((y + 1) * (slices + 1) + x + 1); // 右下

            this->indices.push_back(y * (slices + 1) + x); // 左上
            this->indices.push_back((y + 1) * (slices + 1) + x + 1); // 右下
            this->indices.push_back(y * (slices + 1) + x + 1); // 右上
        }
    }
}
