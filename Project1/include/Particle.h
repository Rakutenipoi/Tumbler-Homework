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
#include "utils.h"

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
	ACCELERATION,
};


class ParticleParameter {
public:
	ParticleParameter();
	friend class ParticleManager;

private:
	vec3 position;
	vec3 direction;
	vec3 acceleration;
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
	int level;

	Particle();
	~Particle();
	Particle& operator=(const Particle& other);

	void update(float deltaTime);
	void init();
	bool isDead();
	virtual void render(Shader shader);

	void setID(int value);
	
	void setParamVector3(vec3 value, ATTRIB_TYPE type);
	void setParamFloat(float value, ATTRIB_TYPE type);
	void setParamInteger(int value, ATTRIB_TYPE type);

	int getID();
	vec3 getParamVector3(ATTRIB_TYPE type);
	float getParamFloat(ATTRIB_TYPE type);
	int getParamInteger(ATTRIB_TYPE type);

protected:
	
	PARTICLE_TYPE type;
	int id;
	
};

class MeshParticle : public Particle {
public:
	MeshParticle();
	MeshParticle& operator=(const MeshParticle& other);

	float mass;
	

	void render(Shader shader) override;

	void setRadius(float value);
	void setMesh(StaticMesh* mesh, MESH_TYPE type);

	float getRadius();
private:
	StaticMesh* mesh;
	float radius;
};

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();

	bool isFalling;
	bool isCheckClash;
	bool isCheckBoundary;
	float friction;

	void add(vector<Particle*> particles);
	void update(float deltaTime);
	void render(Shader shader);
	bool checkBoundary(MeshParticle& target);
	void stop();
	void erase(int idx);
	void erase(Particle* target);
	void applyAcceleration(vec3 value);

	void setBoundary(vec2 x, vec2 y, vec2 z);
	void setBoundary(vec2 xyz);
	void setBoundary(float xyz);

	int getSize();
	vector<Particle*> getParticles();

private:
	vector<Particle*> particles;
	float bounds[6]; // -x, +x, -y, +y, -z, +z
};

class ParticleEmitter {
public:
	ParticleEmitter();
	float velocityTolerance;
	float velocityInitialValue;
	vec3 positionTolerance;
	vec3 positionInitialValue;

	void generate(int num, Particle target, ParticleSystem& ps);
	void generate(int num, MeshParticle target, ParticleSystem& ps);
	void generate(int num, MeshParticle target, ParticleSystem& ps, vec3 direction);
};

