#include "../include/StaticMesh.h"

#define M_PI 3.1415926

void StaticSphere::generate()
{
    // 创建球面顶点(共有(slices + 1) * (stacks + 1)个顶点)
    for (int y = 0; y <= stacks; y++) {
        float alpha = (float)y / (float)stacks * M_PI;
        for (int x = 0; x <= slices; x++) {
            float beta = (float)x / (float)slices * 2 * M_PI;
            float xPos = sin(alpha) * cos(beta);
            float yPos = cos(alpha);
            float zPos = sin(alpha) * sin(beta);

            vec3 posVector = vec3(xPos, yPos, zPos);

            this->vertices.push_back(this->radius * posVector);
            this->normals.push_back(posVector);
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

StaticSphere::StaticSphere(float radius, int slices, int stacks)
{
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;

    this->generate();
}

void StaticSphere::draw()
{
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->slices * this->stacks * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

StaticMesh::StaticMesh()
{
    
}

void StaticMesh::generate()
{
}

void StaticMesh::draw()
{
}

void StaticMesh::clear()
{
    this->vertices.clear();
    this->normals.clear();
    this->indices.clear();
}

void StaticMesh::initBuffer()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
