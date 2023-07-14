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
	bool intersect(AABB* other);
	glm::vec3 getMinPoint();
	glm::vec3 getMaxPoint();
	glm::vec3 getMin();
	glm::vec3 getMax();
	glm::vec3 getCenter();
	void setCenter(glm::vec3 value);
	glm::mat4 update(glm::mat4 model);

protected:
	glm::vec3 minPoint; // ��Χ�е���С������
	glm::vec3 maxPoint; // ��Χ�е���������
	glm::vec3 physMinPoint; // ��Χ������������ϵ����С������
	glm::vec3 physMaxPoint; // ��Χ������������ϵ����������
	glm::vec3 center; // ��Χ������Χ����ı任����
};

