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

	// updateParticle<<<256, 256>>>(particleData_d, deltaTime_d);

	// transfer the data from device memory to host memory
	cudaMemcpy(particleData, particleData_d, sizeof(ParticleParameter), cudaMemcpyDeviceToHost);

	// delete the device memory
	cudaFree(particleData_d);
	cudaFree(deltaTime_d);
}

__device__ void updateParticle(ParticleParameter * particleData, float* deltaTime) {
	vec3 position = particleData->position;
	vec3 direction = particleData->direction;
	vec3 acceleration = particleData->acceleration;
	float velocity = particleData->velocity;

	vec3 newVelocityVector = velocity * direction + acceleration * *deltaTime;
	vec3 newDirection = glm::normalize(newVelocityVector);
	float newVelocity = glm::length(newVelocityVector);

	position += *deltaTime * newVelocityVector;
	particleData->position = position;
	particleData->direction = newDirection;
	particleData->velocity = newVelocity;
}

void updateParticleSystem(Particle** particlesData, float* deltaTime, int numParticles) {
	Particle* particlePtr_d;
	Particle* particlePtr_h = new Particle[numParticles];
	float* deltaTime_d;
	int* numParticles_d;
	size_t particlesSize = numParticles * sizeof(Particle*);
	size_t particleSize = numParticles * sizeof(Particle);

	for (int i = 0; i < numParticles; i++) {
		cudaMemcpy(&particlePtr_h[i], particlesData[i], sizeof(Particle), cudaMemcpyHostToHost);
	}

	// allocate device memory
	//cudaMalloc((void**)&particlesPtr_d, particlesSize);
	cudaMalloc((void**)&particlePtr_d, particleSize);
	cudaMalloc((void**)&deltaTime_d, sizeof(float));
	cudaMalloc((void**)&numParticles_d, sizeof(int));

	// transfer the data from host to device
	cudaMemcpy(particlePtr_d, particlePtr_h, particleSize, cudaMemcpyHostToDevice);
	cudaMemcpy(deltaTime_d, deltaTime, sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(numParticles_d, &numParticles, sizeof(int), cudaMemcpyHostToDevice);

	updateParticleSystem<<<256, 512>>>(particlePtr_d, deltaTime_d, numParticles_d);

	// transfer the data from device to host
	cudaMemcpy(particlePtr_h, particlePtr_d, particleSize, cudaMemcpyDeviceToHost);

	for (int i = 0; i < numParticles; i++) {
		cudaMemcpy(particlesData[i], &particlePtr_h[i], sizeof(Particle), cudaMemcpyHostToHost);
	}

	// delete device memory
	cudaFree(particlePtr_d);
	cudaFree(deltaTime_d);
	cudaFree(numParticles_d);
	delete[] particlePtr_h;
}

__global__ void updateParticleSystem(Particle* particleData, float* deltaTime, int* numParticles) {
	int idx = threadIdx.x + blockIdx.x * blockDim.x;

	if (idx < *numParticles) {
		Particle* particle = &particleData[idx];
		updateParticle(&particle->param, deltaTime);
	}
}

