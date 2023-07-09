#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class AABB
{
public:
	AABB(glm::vec3 minPoint, glm::vec3 maxPoint);
	bool intersect(AABB other);
	glm::vec3 getMinPoint();
	glm::vec3 getMaxPoint();

protected:
	glm::vec3 minPoint; // ��Χ�е���С������
	glm::vec3 maxPoint; // ��Χ�е���������

};
