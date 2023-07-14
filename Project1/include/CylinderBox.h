#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "PhysSphere.h"

// 划分碰撞的区域，不倒翁由下半圆、圆台和上半圆组成
enum class BOUND_STAGE
{
	DOWN,
	MIDDLE,
	UP
};

class CylinderBox
{
public:
	CylinderBox(glm::vec3 axisStart, glm::vec3 axisEnd);
	bool intersect(PhysSphere* sphere, glm::vec3& hit_normal);
	void update(glm::mat4 model);
	glm::vec3* getPhysAxis() {
		return this->physAxis;
	}
	glm::vec3* getAxis() {
		return this->axis;
	}
	void setCenter(glm::vec3* center) {
		this->center[0] = center[0];
		this->center[1] = center[1];
	}
	void setRadiusDown(float radius) {
		this->radius_down = radius;
	}
	void setRadiusUp(float radius) {
		this->radius_up = radius;
	}
	float getRadiusDown() {
		return this->radius_down;
	}
	float getRadiusUp() {
		return this->radius_up;
	}
	// 计算小球中心与中轴线的距离
	float countDistance(glm::vec3 point);
	// 计算小球中心在中轴线上的投影
	glm::vec3 countVectorInAxis(glm::vec3 point);
	// 计算插值比率
	float countInterpolateRate(float length);

protected:
	glm::vec3 axis[2];
	glm::vec3 center[2];
	glm::vec3 physAxis[2];
	glm::vec3 physCenter[2];
	float radius_down;
	float radius_up;
};

