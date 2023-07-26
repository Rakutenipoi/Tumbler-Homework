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

// ������ײ�����򣬲��������°�Բ��Բ̨���ϰ�Բ���
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
	bool intersect(PhysSphere* sphere, glm::vec4& hitNormal);
	bool intersect(glm::vec3 buttomPos, glm::vec3 upPos, float radius, glm::vec3& hitNormal);
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
		this->radius_d = radius;
	}
	void setRadiusUp(float radius) {
		this->radius_u = radius;
	}
	void setScale(float scale) {
		this->scale = scale;
	}
	float getRadiusDown() {
		return this->radius_down;
	}
	float getRadiusUp() {
		return this->radius_up;
	}
	// ����С�������������ߵľ���
	float countDistance(glm::vec3 point);
	// ����С���������������ϵ�ͶӰ
	glm::vec3 countVectorInAxis(glm::vec3 point);
	// �����ֵ����
	float countInterpolateRate(float length);

	glm::vec3 axis[2];
	glm::vec3 center[2];
	glm::vec3 physAxis[2];
	glm::vec3 physCenter[2];
	float radius_down;
	float radius_up;
	float radius_d;
	float radius_u;
	float centerLength;
	float axisLength;
	float scale;
};

