#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Plane.h"
#include "Shader.h"
#include "Sphere.h"
#include "AABB.h"

class PhysSphere : public Sphere
{
public:
    PhysSphere(glm::vec3 position, float radius, int slices, int stacks, float mass);
    ~PhysSphere();
    glm::mat4 update(float deltaTime);

    void setPos(glm::vec3 value);
    void setVel(glm::vec3 value);
    void setAcc(glm::vec3 value);

    bool stop;
    AABB* aabb;

protected:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
};

