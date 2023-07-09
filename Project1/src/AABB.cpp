#include "../include/AABB.h"

AABB::AABB(glm::vec3 minPoint, glm::vec3 maxPoint)
{
	this->minPoint = minPoint;
	this->maxPoint = maxPoint;
}

bool AABB::intersect(AABB other)
{
	glm::vec3 minA = this->getMinPoint();
	glm::vec3 maxA = this->getMaxPoint();
	glm::vec3 minB = other.getMinPoint();
	glm::vec3 maxB = other.getMaxPoint();

	// 检查每个轴上是否有重叠
	if (maxA.x < minB.x || minA.x > maxB.x) {
		return false;
	}
	else if (maxA.y < minB.y || minA.y > maxB.y) {
		return false;
	}
	else if (maxA.z < minB.z || minA.z > maxB.z) {
		return false;
	}

	return true;
}

glm::vec3 AABB::getMinPoint()
{
	return this->minPoint;
}

glm::vec3 AABB::getMaxPoint()
{
	return this->maxPoint;
}
