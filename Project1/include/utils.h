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

// ����0��1�������
float generateRandomNumber();

// ����һ��������������������н�Ϊalpha���ĸ��������Ƕȵ�λΪ��
std::vector<glm::vec3> vectorSplit(glm::vec3 direction, float alpha);
