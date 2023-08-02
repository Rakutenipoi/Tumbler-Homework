#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cuda_runtime.h>
#include<device_launch_parameters.h>

#include "Particle.h"

using namespace std;
using namespace glm;

extern "C" void updateParticle_h(ParticleParameter * particleData, float* deltaTime);
__global__ void updateParticle(ParticleParameter * particleData, float* deltaTime);


