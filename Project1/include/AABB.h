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
	glm::vec3 minPoint; // 包围盒的最小点坐标
	glm::vec3 maxPoint; // 包围盒的最大点坐标
	glm::vec3 physMinPoint; // 包围盒在世界坐标系的最小点坐标
	glm::vec3 physMaxPoint; // 包围盒在世界坐标系的最大点坐标
	glm::vec3 center; // 包围盒所包围物体的变换中心
};

