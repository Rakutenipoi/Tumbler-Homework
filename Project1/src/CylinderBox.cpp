#include "../include/CylinderBox.h"

CylinderBox::CylinderBox(glm::vec3 axisStart, glm::vec3 axisEnd)
{
	this->axis[0] = axisStart;
	this->axis[1] = axisEnd;
}

bool CylinderBox::intersect(PhysSphere* sphere, glm::vec3 &hit_normal)
{
	// 小球中心坐标和半径
	glm::vec3 position = sphere->getPos();
	float radiusSphere = sphere->getRadius();

	glm::vec3 axisVec = this->physAxis[1] - this->physAxis[0];

	// 计算小球中心在中轴线上的投影
	glm::vec3 vectorInAxis = this->countVectorInAxis(position);
	float shadowLength = glm::dot(vectorInAxis, glm::normalize(axisVec));

	// 计算小球中心与中轴线的距离
	float distance = this->countDistance(position);

	if (shadowLength <= -radiusSphere) {
		return false;
	}
	else if (shadowLength <= this->radius_down) {
		// DOWN
		float distanceCenter = glm::length(position - this->physCenter[0]);

		if (distanceCenter <= radiusSphere + radius_down) {
			hit_normal = glm::normalize(position - this->physCenter[0]);
			return true;
		}
		else {
			return false;
		}
	} 
	else if (shadowLength <= glm::length(this->physCenter[1] - this->physAxis[0])) {
		// MIDDLE
		float radiusMiddle = this->radius_down + this->countInterpolateRate(shadowLength) * (this->radius_up - this->radius_down);
		
		if (distance <= radiusMiddle + radiusSphere) {
			hit_normal = glm::normalize(position - (this->physAxis[0] + shadowLength * glm::normalize(axisVec)));
			return true;
		} 
		else {
			return false;
		}
	} 
	else if (shadowLength <= axisVec.length() + radiusSphere) {
		// UP
		float distanceCenter = glm::length(position - this->physCenter[1]);

		if (distanceCenter <= radiusSphere + radius_up) {
			hit_normal = glm::normalize(position - this->physCenter[1]);
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

void CylinderBox::update(glm::mat4 model)
{
	this->physAxis[0] = glm::vec3(model * glm::vec4(this->axis[0], 1.0f));
	this->physAxis[1] = glm::vec3(model * glm::vec4(this->axis[1], 1.0f));
	this->physCenter[0] = glm::vec3(model * glm::vec4(this->center[0], 1.0f));
	this->physCenter[1] = glm::vec3(model * glm::vec4(this->center[1], 1.0f));
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
	return glm::length((length - this->radius_down) / (this->center[1] - this->center[0]));
}





