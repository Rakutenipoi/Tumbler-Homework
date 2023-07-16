#include "../include/CylinderBox.h"

CylinderBox::CylinderBox(glm::vec3 axisStart, glm::vec3 axisEnd)
{
	this->axis[0] = axisStart;
	this->axis[1] = axisEnd;
}

bool CylinderBox::intersect(PhysSphere* sphere, glm::vec4 &hitNormal)
{
	// 小球中心坐标和半径
	glm::vec3 position = sphere->getPos();
	float radiusSphere = sphere->getRadius();

	glm::vec3 axisVec = this->physAxis[1] - this->physAxis[0];
	glm::vec3 down2Point = position - this->physAxis[0];

	// 计算小球中心在中轴线上的投影
	float shadowLength = glm::dot(glm::normalize(axisVec), down2Point);

	// 计算小球中心与中轴线的距离
	float distance = sqrt(pow(glm::length(down2Point), 2) - pow(shadowLength, 2));

	glm::vec3 hit_normal;

	if (shadowLength <= -radiusSphere) {
		return false;
	}
	else if (shadowLength <= this->radius_down) {
		// DOWN
		float distanceCenter = glm::length(position - this->physCenter[0]);

		if (distanceCenter <= (radiusSphere + radius_down)) {
			hit_normal = glm::normalize(position - this->physCenter[0]);
			hitNormal = glm::vec4(hit_normal, -0.5f);
			return true;
		}
		else {
			return false;
		}
	} 
	else if (shadowLength <= glm::length(this->physCenter[1] - this->physAxis[0])) {
		// MIDDLE
		float interpolateRate = this->countInterpolateRate(shadowLength);
		float radiusMiddle = this->radius_down + interpolateRate * (this->radius_up - this->radius_down);
		
		if (distance <= (radiusMiddle + 2 * radiusSphere)) {
			hit_normal = glm::normalize(position - (this->physCenter[0] + shadowLength * glm::normalize(axisVec)));
			hitNormal = glm::vec4(hit_normal, interpolateRate + 0.3f);
			return true;
		} 
		else {
			return false;
		}
	} 
	else if (shadowLength <= glm::length(axisVec) + radiusSphere) {
		// UP
		float distanceCenter = glm::length(position - this->physCenter[1]);

		if (distanceCenter <= (radiusSphere + radius_up)) {
			hit_normal = glm::normalize(position - this->physCenter[1]);
			hitNormal = glm::vec4(hit_normal, 1.3f);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}

}

bool CylinderBox::intersect(glm::vec3 position, float radius, glm::vec3& hitNormal)
{
	glm::vec2 point2point = glm::vec2(position.x, position.z) - glm::vec2(this->physAxis[0].x, this->physAxis[0].z);
	float distance = glm::length(point2point);

	if (distance <= (this->radius_down + radius) * 1.05f) {
		hitNormal = glm::normalize(glm::vec3(point2point.x, 0.0f, point2point.y));
		return true;
	}

	return false;
}

void CylinderBox::update(glm::mat4 model)
{
	this->physAxis[0] = glm::vec3(model * glm::vec4(this->axis[0], 1.0f));
	this->physAxis[1] = glm::vec3(model * glm::vec4(this->axis[1], 1.0f));
	this->physCenter[0] = glm::vec3(model * glm::vec4(this->center[0], 1.0f));
	this->physCenter[1] = glm::vec3(model * glm::vec4(this->center[1], 1.0f));
	this->radius_down = this->scale * this->radius_d;
	this->radius_up = this->scale * this->radius_u;
	this->axisLength = glm::length(this->physAxis[1] - this->physAxis[0]);
	this->centerLength = glm::length(this->physCenter[1] - this->physCenter[0]);
}

float CylinderBox::countDistance(glm::vec3 point)
{
	glm::vec3 down2Point = point - this->physAxis[0];
	glm::vec3 distanceVec = down2Point - this->countVectorInAxis(point);

	return glm::length(distanceVec);
}

glm::vec3 CylinderBox::countVectorInAxis(glm::vec3 point)
{
	glm::vec3 down2Point = point - this->physAxis[0];
	glm::vec3 axisVec = this->physAxis[1] - this->physAxis[0];

	return glm::dot(axisVec, down2Point) * glm::normalize(axisVec);
}

float CylinderBox::countInterpolateRate(float length)
{
	return (length - this->radius_down) / this->centerLength;
}





