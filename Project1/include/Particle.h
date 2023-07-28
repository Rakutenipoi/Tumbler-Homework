#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "StaticMesh.h"
#include "Shader.h"

using namespace glm;
using namespace std;

enum PARTICLE_TYPE
{
	BILLBOARD,
	MESH,
};

enum ATTRIB_TYPE
{
	POSITION,
	DIRECTION,
	VELOCITY,
	LIFESPAN,
	COLOR,
	ALPHA,
};


class ParticleParameter {
public:
	ParticleParameter();
	friend class ParticleManager;

private:
	vec3 position;
	vec3 direction;
	float velocity;
	int lifeSpan;
	vec3 color;
	float alpha;
};


class ParticleManager {
public:
	static void setInteger(int value, ParticleParameter& target, ATTRIB_TYPE type);
	static void setFloat(float value, ParticleParameter& target, ATTRIB_TYPE type);
	static void setVector3(vec3 value, ParticleParameter& target, ATTRIB_TYPE type);

	static vec3 getVector3(ParticleParameter& target, ATTRIB_TYPE type);
	static float getFloat(ParticleParameter& target, ATTRIB_TYPE type);
	static int getInteger(ParticleParameter& target, ATTRIB_TYPE type);
};


class Particle {
public:
	ParticleParameter param;

	Particle();
	~Particle();
	void update(float deltaTime);
	void init();
	bool isDead();
	void setMesh(MESH_TYPE type);
	void setID(int value);
	void setRadius(float value);
	int getID();
	void render(Shader shader);

	void setParamVector3(vec3 value, ATTRIB_TYPE type);
	void setParamFloat(float value, ATTRIB_TYPE type);
	void setParamInteger(int value, ATTRIB_TYPE type);

	vec3 getParamVector3(ATTRIB_TYPE type);
	float getParamFloat(ATTRIB_TYPE type);
	int getParamInteger(ATTRIB_TYPE type);

private:
	StaticMesh* mesh;
	PARTICLE_TYPE type;
	int id;
	float radius;
};


class ParticleSystem {
public:
	ParticleSystem();
	void add(vector<Particle> particles);
	void update(float deltaTime);
	void render(Shader shader);
	void checkBoundary(Particle target);

private:
	vector<Particle> particles;
	float bounds[6];
};

class ParticleEmitter {
public:
	ParticleEmitter();
	float velocityTolerance;
	float velocityInitialValue;
	vec3 positionTolerance;
	vec3 positionInitialValue;

	void generate(int num);
};

