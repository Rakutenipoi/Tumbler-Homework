#include "../include/AABB.h"

AABB::AABB(glm::vec3 minPoint, glm::vec3 maxPoint)
{
	this->minPoint = minPoint;
	this->maxPoint = maxPoint;
	this->setCenter(glm::vec3(0.0f));
}

bool AABB::intersect(AABB* other)
{
	glm::vec3 minA = this->getMinPoint();
	glm::vec3 maxA = this->getMaxPoint();
	glm::vec3 minB = other->getMinPoint();
	glm::vec3 maxB = other->getMaxPoint();

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
	return this->physMinPoint;
}

glm::vec3 AABB::getMaxPoint()
{
	return this->physMaxPoint;
}

glm::vec3 AABB::getCenter()
{
	return this->center;
}

void AABB::setCenter(glm::vec3 value)
{
	this->center = value;
}

glm::mat4 AABB::update(glm::mat4 model)
{
	glm::mat4 _model = glm::mat4(1.0f);
	for (int i = 0; i < 3; i++) {
		_model[3][i] = model[3][i];
	}

	this->physMinPoint = glm::vec3(_model * glm::vec4(this->minPoint, 1.0f));
	this->physMaxPoint = glm::vec3(_model * glm::vec4(this->maxPoint, 1.0f));

	return _model;
}
