#include "../include/Particle.h"

#define DEFAULT_SLICES 30
#define DEFAULT_STACKS 30
#define DEFAULT_RADIUS 0.01f

ParticleParameter::ParticleParameter()
{

}

Particle::Particle()
{
	this->radius = DEFAULT_RADIUS;
}

Particle::~Particle()
{
	//delete this->mesh;
}

void Particle::update(float deltaTime)
{
	vec3 position = this->getParamVector3(ATTRIB_TYPE::POSITION);
	vec3 direction = this->getParamVector3(ATTRIB_TYPE::DIRECTION);
	float velocity = this->getParamFloat(ATTRIB_TYPE::VELOCITY);

	position += velocity * direction;
	this->setParamVector3(position, ATTRIB_TYPE::POSITION);
}

void Particle::init()
{

}

bool Particle::isDead()
{
	return this->getParamInteger(ATTRIB_TYPE::LIFESPAN) <= 0 ? true : false;
}

void Particle::setMesh(MESH_TYPE type)
{
	switch (type)
	{
	case MESH_TYPE::SPHERE:
		mesh = new StaticSphere(this->radius, DEFAULT_SLICES, DEFAULT_STACKS);
		break;
	case MESH_TYPE::CUBE:
		break;
	default:
		break;
	}

	mesh->type = type;
}

void Particle::setID(int value)
{
	this->id = value;
}

void Particle::setRadius(float value)
{
	this->radius = value;
}

int Particle::getID()
{
	return this->id;
}

void Particle::render(Shader shader)
{
	mat4 model = translate(mat4(1.0f), getParamVector3(ATTRIB_TYPE::POSITION));
	shader.setMatrix4("model", model);
	shader.setFloat("alpha", getParamFloat(ATTRIB_TYPE::ALPHA));
	shader.setVector3("color", getParamVector3(ATTRIB_TYPE::COLOR));

	if (this->mesh->type == MESH_TYPE::SPHERE) {
		StaticSphere* sphere = dynamic_cast<StaticSphere*>(this->mesh);
		sphere->draw();
	}
	
}

void Particle::setParamVector3(vec3 value, ATTRIB_TYPE type)
{
	switch (type)
	{
	case POSITION:
		ParticleManager::setVector3(value, this->param, type);
		break;
	case DIRECTION:
		ParticleManager::setVector3(value, this->param, type);
		break;
	case COLOR:
		ParticleManager::setVector3(value, this->param, type);
		break;
	default:
		break;
	}
}

void Particle::setParamFloat(float value, ATTRIB_TYPE type)
{
	switch (type)
	{
	case VELOCITY:
		ParticleManager::setFloat(value, this->param, type);
		break;
	case ALPHA:
		ParticleManager::setFloat(value, this->param, type);
		break;
	default:
		break;
	}
}

void Particle::setParamInteger(int value, ATTRIB_TYPE type)
{
	switch (type)
	{
	case LIFESPAN:
		ParticleManager::setInteger(value, this->param, type);
		break;
	default:
		break;
	}
}

vec3 Particle::getParamVector3(ATTRIB_TYPE type)
{
	return ParticleManager::getVector3(this->param, type);
}

float Particle::getParamFloat(ATTRIB_TYPE type)
{
	return ParticleManager::getFloat(this->param, type);
}

int Particle::getParamInteger(ATTRIB_TYPE type)
{
	return ParticleManager::getInteger(this->param, type);
}

ParticleSystem::ParticleSystem()
{
}

void ParticleSystem::add(vector<Particle> particles)
{
	for (Particle particle : particles) {
		this->particles.push_back(particle);
	}
}

void ParticleSystem::update(float deltaTime)
{
	for (Particle particle : this->particles) {
		particle.update(deltaTime);
	}
}

void ParticleSystem::render(Shader shader)
{
	for (Particle particle : this->particles) {
		particle.render(shader);
	}
}

void ParticleManager::setInteger(int value, ParticleParameter& target, ATTRIB_TYPE type)
{
	switch (type)
	{
	case LIFESPAN:
		target.lifeSpan = value;
		break;
	default:
		break;
	}
}

void ParticleManager::setFloat(float value, ParticleParameter& target, ATTRIB_TYPE type)
{
	switch (type)
	{
	case VELOCITY:
		target.velocity = value;
		break;
	case ALPHA:
		target.alpha = value;
		break;
	default:
		break;
	}
}

void ParticleManager::setVector3(vec3 value, ParticleParameter& target, ATTRIB_TYPE type)
{
	switch (type)
	{
	case POSITION:
		target.position = value;
		break;
	case DIRECTION:
		target.direction = value;
		break;
	case COLOR:
		target.color = value;
		break;
	default:
		break;
	}
}

vec3 ParticleManager::getVector3(ParticleParameter& target, ATTRIB_TYPE type)
{
	vec3 returnValue;

	switch (type)
	{
	case POSITION:
		returnValue = target.position;
		break;
	case DIRECTION:
		returnValue = target.direction;
		break;
	case COLOR:
		returnValue = target.color;
		break;
	default:
		break;
	}

	return returnValue;
}

float ParticleManager::getFloat(ParticleParameter& target, ATTRIB_TYPE type)
{
	float returnValue;

	switch (type)
	{
	case VELOCITY:
		returnValue = target.velocity;
		break;
	case ALPHA:
		returnValue = target.alpha;
		break;
	default:
		break;
	}

	return returnValue;
}

int ParticleManager::getInteger(ParticleParameter& target, ATTRIB_TYPE type)
{
	int returnValue;

	switch (type)
	{
	case LIFESPAN:
		returnValue = target.lifeSpan;
		break;
	default:
		break;
	}

	return returnValue;
}

ParticleEmitter::ParticleEmitter()
{
}
