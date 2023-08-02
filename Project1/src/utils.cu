#include "../include/utils.cuh"

void updateParticle_h(ParticleParameter* particleData, float* deltaTime) {
	ParticleParameter* particleData_d;
	float* deltaTime_d;

	// allocate devide memory
	cudaMalloc((void**)&particleData_d, sizeof(ParticleParameter));
	cudaMalloc((void**)&deltaTime_d, sizeof(float));

	// transfer the data from host memory to device memory
	cudaMemcpy(particleData_d, particleData, sizeof(ParticleParameter), cudaMemcpyHostToDevice);
	cudaMemcpy(deltaTime_d, deltaTime, sizeof(float), cudaMemcpyHostToDevice);

	updateParticle<<<4, 6>>>(particleData_d, deltaTime_d);

	// transfer the data from device memory to host memory
	cudaMemcpy(particleData, particleData_d, sizeof(ParticleParameter), cudaMemcpyDeviceToHost);

	// delete the device memory
	cudaFree(particleData_d);
	cudaFree(deltaTime_d);
}

__global__ void updateParticle(ParticleParameter * particleData, float* deltaTime) {
	vec3 position = particleData->position;
	vec3 direction = particleData->direction;
	vec3 acceleration = particleData->acceleration;
	float velocity = particleData->velocity;

	vec3 newVelocityVector = velocity * direction + acceleration * *deltaTime;
	vec3 newDirection = glm::normalize(newVelocityVector);
	float newVelocity = glm::length(newVelocityVector);

	position += newVelocityVector * *deltaTime;

	particleData->position = position;
	particleData->direction = newDirection;
	particleData->velocity = newVelocity;
}

