#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <glm/gtc/random.hpp>

#include "Shader.h"

void lineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color, Shader shader);
void pointsDraw(std::vector<glm::vec3> pts, glm::vec3 color, Shader shader);
void triangleDraw(std::vector<glm::vec3> pts, glm::vec3 color, Shader shader);
glm::mat4 TranslateTowardsDirection(glm::vec3& direction, const float distance);
void PrintMat4(const glm::mat4& matrix);

// 生成0到1的随机数
float generateRandomNumber();

// 输入一个向量，返回与该向量夹角为alpha的四个向量，角度单位为度
std::vector<glm::vec3> vectorSplit(glm::vec3 direction, float alpha);
