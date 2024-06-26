#include "../include/PhysModel.h"

#define G 0.96f
#define FRICTION_ANGLE 0.005f

PhysModel::PhysModel(glm::vec3 position, float mass, char* path)
{
	this->loadModel(path);
	this->aabb = new AABB(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.107f, 0.0f)); // 0.1, 0.215, 0.1
	this->cylinder = new CylinderBox(glm::vec3(0.0f, -0.05f, 0.0f), glm::vec3(0.0f, 0.107f, 0.0f));
	this->cylinder->setRadiusDown(0.05f);
	this->cylinder->setRadiusUp(0.025f);
	glm::vec3 centerCylinder[2] = { glm::vec3(0.0f), glm::vec3(0.0f, 0.082f, 0.0f) };
	this->cylinder->setCenter(centerCylinder);
	this->stop = false;
	this->position = position;
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
	this->position_angle = glm::vec3(0.0f);
	this->velocity_angle = glm::vec3(0.0f);
	this->acceleration_angle = glm::vec3(0.0f);
	this->friction_angle = FRICTION_ANGLE;
	this->torque_length = 10.0f;
	this->mass = mass;
	this->scale = 1.5f;
	this->cylinder->setScale(this->scale);
}

glm::mat4 PhysModel::update(float deltaTime)
{
	// λ��
	this->velocity *= (1 - this->friction);
	this->velocity += deltaTime * this->acceleration;
	this->position += deltaTime * this->velocity;

	
	// ˥��
	float threadAngle_x = 1.0f;
	float threadAngle_z = 1.0f;
	float upThread = 70.0f;
	float downThread = 30.0f;
	float rotateThread = 200.0f;
	if (abs(this->position_angle.x) >= upThread) {
		threadAngle_x = 0.0f;
	}
	else if (abs(this->position_angle.x) >= downThread) {
		threadAngle_x = ((abs(this->position_angle.x) - downThread) / (upThread - downThread) + 1) / 2;
	}
	else {
		threadAngle_x = 1.0f;
	}

	if (abs(this->position_angle.z) >= upThread) {
		threadAngle_z = 0.0f;
	}
	else if (abs(this->position_angle.z) >= downThread) {
		threadAngle_z = ((abs(this->position_angle.z) - downThread) / (upThread - downThread) + 1) / 2;
	}
	else {
		threadAngle_z = 1.0f;
	}

	// ������ٶ�
	this->acceleration_angle = -glm::vec3(this->position_angle.x, 0.0f, this->position_angle.z) * this->torque_length;
	// �Ƕ�˥��
	this->position_angle = glm::vec3(this->position_angle.x * (1 - this->friction_angle), this->position_angle.y, this->position_angle.z * (1 - this->friction_angle));
	// �ٶ�˥��
	this->velocity_angle = glm::vec3(this->velocity_angle.x * threadAngle_x, glm::clamp(this->velocity_angle.y * (1 - this->friction_angle), -rotateThread, rotateThread)
		, this->velocity_angle.z * threadAngle_z);

	// �Ƕ�
	this->velocity_angle += deltaTime * this->acceleration_angle;
	this->position_angle += deltaTime * this->velocity_angle;
	

	// �߽�ֵ
	float bound = 0.5f - this->scale * this->cylinder->radius_d;

	// �߽���ײ�ж�
	if (abs(this->position.x) > bound) {
		this->position.x = bound * this->position.x / abs(this->position.x);
		this->velocity.x *= -1;
	}
	else if (abs(this->position.y) > bound) {
		this->position.y = bound * this->position.y / abs(this->position.y);
		this->velocity.y *= -1;
	}
	else if (abs(this->position.z) > bound) {
		this->position.z = bound * this->position.z / abs(this->position.z);
		this->velocity.z *= -1;
	}

	glm::mat4 model = glm::mat4(1.0f);
	// ƽ��
	model = glm::translate(model, this->position);
	// ��X����ת
	model = glm::rotate(model, glm::radians(this->position_angle.x), glm::vec3(1.0f, 0.0f, 0.0f));
	// ��Y����ת
	model = glm::rotate(model, glm::radians(this->position_angle.y), glm::vec3(0.0f, 1.0f, 0.0f));
	// ��Z����ת
	model = glm::rotate(model, glm::radians(this->position_angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// ����
	model = glm::scale(model, glm::vec3(this->scale));

	return model;
}

void PhysModel::setPos(glm::vec3 value)
{
	this->position = value;
}

void PhysModel::setVel(glm::vec3 value)
{
	this->velocity = value;
}

void PhysModel::setAcc(glm::vec3 value)
{
	this->acceleration = value;
}

void PhysModel::setPosAngle(glm::vec3 value)
{
	this->position_angle = value;
}

void PhysModel::setVelAngle(glm::vec3 value)
{
	this->velocity_angle = value;
}

void PhysModel::setAccAngle(glm::vec3 value)
{
	this->acceleration_angle = value;
}

void PhysModel::setFric(float value)
{
	this->friction = value;
}

void PhysModel::apply(glm::vec3 direction, float value)
{

}

void PhysModel::addValue(glm::vec3 value, PHYS_PARAM_TYPE type)
{
	switch (type)
	{
	case PHYS_PARAM_TYPE::POSITION:
		this->position += value;
		break;
	case PHYS_PARAM_TYPE::VELOCITY:
		this->velocity += value;
		break;
	case PHYS_PARAM_TYPE::ACCELERATION:
		this->acceleration += value;
		break;
	case PHYS_PARAM_TYPE::ANGLE_POSITION:
		this->position_angle += value;
		break;
	case PHYS_PARAM_TYPE::ANGLE_VELOCITY:
		this->velocity_angle += value;
		break;
	case PHYS_PARAM_TYPE::ANGLE_ACCELERATION:
		this->acceleration_angle += value;
		break;
	default:
		break;
	}
}


