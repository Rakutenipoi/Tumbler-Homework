#include "../include/Particle.h"

#define DEFAULT_RADIUS 0.01f
#define DEFAULT_LIFESPAN 30

ParticleParameter::ParticleParameter()
{
	this->acceleration = vec3(0.0f);
	this->lifeSpan = DEFAULT_LIFESPAN;
	this->position = vec3(0.0f);
	this->direction = vec3(0.0f);
	this->velocity = 0.0f;
	this->alpha = 1.0f;
	this->color = vec3(1.0f);
}

Particle::Particle()
{
	this->level = 1;
}

Particle::~Particle()
{
	//delete this->mesh;
}

Particle& Particle::operator=(const Particle& other)
{
	this->type = other.type;
	this->level = other.level;

	return *this;
}

MeshParticle& MeshParticle::operator=(const MeshParticle& other)
{
	Particle::operator=(other);

	this->radius = other.radius;
	this->mesh = other.mesh;

	return *this;
}

void Particle::update(float deltaTime)
{
	vec3 position = this->getParamVector3(ATTRIB_TYPE::POSITION);
	vec3 direction = this->getParamVector3(ATTRIB_TYPE::DIRECTION);
	vec3 acceleration = this->getParamVector3(ATTRIB_TYPE::ACCELERATION);
	float velocity = this->getParamFloat(ATTRIB_TYPE::VELOCITY);
	
	vec3 newVelocityVector = velocity * direction + acceleration * deltaTime;
	vec3 newDirection = glm::normalize(newVelocityVector);
	float newVelocity = glm::length(newVelocityVector);

	position += newVelocityVector * deltaTime;

	this->setParamVector3(position, ATTRIB_TYPE::POSITION);
	this->setParamVector3(newDirection, ATTRIB_TYPE::DIRECTION);
	this->setParamFloat(newVelocity, ATTRIB_TYPE::VELOCITY);
}

void Particle::init()
{

}

bool Particle::isDead()
{
	return this->getParamInteger(ATTRIB_TYPE::LIFESPAN) <= 0 ? true : false;
}

void Particle::setID(int value)
{
	this->id = value;
}

void MeshParticle::setRadius(float value)
{
	this->radius = value;
}

int Particle::getID()
{
	return this->id;
}

void Particle::render(Shader shader)
{
}

MeshParticle::MeshParticle()
{
	this->type = PARTICLE_TYPE::MESH;
	this->radius = DEFAULT_RADIUS;
	this->mass = 1.0f;
}

void MeshParticle::render(Shader shader)
{
	mat4 model = translate(mat4(1.0f), getParamVector3(ATTRIB_TYPE::POSITION));
	model = glm::scale(model, glm::vec3(this->radius));
	shader.setMatrix4("model", model);
	shader.setFloat("alpha", getParamFloat(ATTRIB_TYPE::ALPHA));
	shader.setVector3("color", getParamVector3(ATTRIB_TYPE::COLOR));

	if (this->mesh->type == MESH_TYPE::SPHERE) {
		StaticSphere* sphere = dynamic_cast<StaticSphere*>(this->mesh);
		sphere->draw();
	}

}

float MeshParticle::getRadius()
{
	return this->radius;
}

void MeshParticle::setMesh(StaticMesh* mesh, MESH_TYPE type)
{
	switch (type)
	{
	case MESH_TYPE::SPHERE:
		this->mesh = mesh;
		break;
	case MESH_TYPE::CUBE:
		break;
	default:
		break;
	}

	mesh->type = type;
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
	case ACCELERATION:
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
	this->isFalling = false;
	this->friction = 0.0f;
	this->isCheckClash = false;
	this->isCheckBoundary = true;
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* particle : this->particles) {
		delete particle;
	}
}

void ParticleSystem::add(vector<Particle*> particles)
{
	for (Particle* particle : particles) {
		this->particles.push_back(particle);
	}
}

void ParticleSystem::update(float deltaTime)
{
	for (int i = 0; i < this->particles.size(); i++) {
		Particle* particle = particles.at(i);
		particle->update(deltaTime);
		if (isCheckBoundary) {
			MeshParticle* meshParticle = dynamic_cast<MeshParticle*>(particle);

			bool hitGround = this->checkBoundary(*meshParticle);
			if (hitGround) {
				this->erase(i);
			}
		}
	}
}

void ParticleSystem::render(Shader shader)
{
	for (int i = 0; i < this->particles.size(); i++) {
		particles.at(i)->render(shader);
	}
}

bool ParticleSystem::checkBoundary(MeshParticle& target)
{
	float radius = target.getRadius();
	vec3 position = target.getParamVector3(ATTRIB_TYPE::POSITION);
	vec3 direction = target.getParamVector3(ATTRIB_TYPE::DIRECTION);
	float velocity = target.getParamFloat(ATTRIB_TYPE::VELOCITY);
	bool hit = false;

	// X÷·±ﬂΩÁ≈–∂œ
	if (position.x < this->bounds[0] + radius) {
		direction.x *= -1;
		position.x = this->bounds[0] + radius;
		hit = true;
		target.setParamVector3(vec3(0.56f, 0.71f, 0.29f), ATTRIB_TYPE::COLOR);
	}
	else if (position.x > this->bounds[1] - radius) {
		direction.x *= -1;
		position.x = this->bounds[1] - radius;
		hit = true;
		target.setParamVector3(vec3(0.4f, 0.73f, 0.72f), ATTRIB_TYPE::COLOR);
	}

	// Y÷·±ﬂΩÁ≈–∂œ
	if (position.y < this->bounds[2] + radius) {
		if (this->isFalling) {
			return true;
		}
		direction.y *= -1;
		position.y = this->bounds[2] + radius;
		hit = true;
	}
	else if (position.y > this->bounds[3] - radius) {
		direction.y *= -1;
		position.y = this->bounds[3] - radius;
		hit = true;
	}

	// Z÷·±ﬂΩÁ≈–∂œ
	if (position.z < this->bounds[4] + radius) {
		direction.z *= -1;
		position.z = this->bounds[4] + radius;
		target.setParamVector3(vec3(0.98f, 0.89f, 0.32f), ATTRIB_TYPE::COLOR);
		hit = true;
	}
	else if (position.z > this->bounds[5] - radius) {
		direction.z *= -1;
		position.z = this->bounds[5] - radius;
		hit = true;
	}

	if (hit) {
		velocity *= (1.0 - this->friction);
	}

	target.setParamVector3(position, ATTRIB_TYPE::POSITION);
	target.setParamVector3(direction, ATTRIB_TYPE::DIRECTION);
	target.setParamFloat(velocity, ATTRIB_TYPE::VELOCITY);

	return false;
}

void ParticleSystem::stop()
{
	for (Particle* particle : this->particles) {
		particle->setParamFloat(0.0f, ATTRIB_TYPE::VELOCITY);
		particle->setParamVector3(vec3(0.0f), ATTRIB_TYPE::DIRECTION);
		particle->setParamVector3(vec3(0.0f), ATTRIB_TYPE::ACCELERATION);
	}
}

void ParticleSystem::erase(int idx)
{
	Particle* particle = this->particles.at(idx);
	this->particles.erase(this->particles.begin() + idx);
	delete particle;
}

void ParticleSystem::erase(Particle* target)
{
	auto it = find(this->particles.begin(), this->particles.end(), target);
	if (it != this->particles.end()) {
		int idx = std::distance(this->particles.begin(), it);
		this->erase(idx);
	}
}

void ParticleSystem::applyAcceleration(vec3 value)
{
	for (Particle* particle : this->particles) {
		particle->setParamVector3(value, ATTRIB_TYPE::ACCELERATION);
	}
}

void ParticleSystem::setBoundary(vec2 x, vec2 y, vec2 z)
{
	this->bounds[0] = x.x;
	this->bounds[1] = x.y;
	this->bounds[2] = y.x;
	this->bounds[3] = y.y;
	this->bounds[4] = z.x;
	this->bounds[5] = z.y;
}

void ParticleSystem::setBoundary(vec2 xyz)
{
	this->setBoundary(xyz, xyz, xyz);
}

void ParticleSystem::setBoundary(float xyz)
{
	this->setBoundary(vec2(-xyz, xyz));
}

int ParticleSystem::getSize()
{
	return this->particles.size();
}

vector<Particle*> ParticleSystem::getParticles()
{
	return this->particles;
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
	case ACCELERATION:
		target.acceleration = value;
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
	case ACCELERATION:
		returnValue = target.acceleration;
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

void ParticleEmitter::generate(int num, Particle target, ParticleSystem& ps)
{
	
}

void ParticleEmitter::generate(int num, MeshParticle target, ParticleSystem& ps)
{
	vector<Particle*> particles;
	for (int i = 0; i < num; i++) {
		MeshParticle* particle = new MeshParticle();
		*particle = target;
		particle->setParamVector3(target.getParamVector3(ATTRIB_TYPE::COLOR), ATTRIB_TYPE::COLOR);
		particle->setParamFloat(target.getParamFloat(ATTRIB_TYPE::ALPHA), ATTRIB_TYPE::ALPHA);
		particle->setParamVector3(this->positionInitialValue, ATTRIB_TYPE::POSITION);
		vec3 direction = vec3(generateRandomNumber(), generateRandomNumber(), generateRandomNumber()) * 2.0f - 1.0f;
		particle->setParamVector3(glm::normalize(direction), ATTRIB_TYPE::DIRECTION);
		particle->setParamFloat(velocityInitialValue + (2 * generateRandomNumber() - 1) * velocityTolerance, ATTRIB_TYPE::VELOCITY);
		particle->setParamInteger(DEFAULT_LIFESPAN, ATTRIB_TYPE::LIFESPAN);

		particles.push_back(particle);
	}

	ps.add(particles);
}

void ParticleEmitter::generate(int num, MeshParticle target, ParticleSystem& ps, vec3 direction)
{
	vector<Particle*> particles;
	for (int i = 0; i < num; i++) {
		MeshParticle* particle = new MeshParticle();
		*particle = target;
		particle->setParamVector3(target.getParamVector3(ATTRIB_TYPE::COLOR), ATTRIB_TYPE::COLOR);
		particle->setParamFloat(target.getParamFloat(ATTRIB_TYPE::ALPHA), ATTRIB_TYPE::ALPHA);
		particle->setParamVector3(this->positionInitialValue, ATTRIB_TYPE::POSITION);
		vec3 direction = generateRandomDirection(30);
		particle->setParamVector3(glm::normalize(direction), ATTRIB_TYPE::DIRECTION);
		particle->setParamFloat(velocityInitialValue + (2 * generateRandomNumber() - 1) * velocityTolerance, ATTRIB_TYPE::VELOCITY);
		particle->setParamInteger(DEFAULT_LIFESPAN, ATTRIB_TYPE::LIFESPAN);
		particle->setParamVector3(vec3(0.0f, -0.1f, 0.0f), ATTRIB_TYPE::ACCELERATION);

		particles.push_back(particle);
	}

	ps.add(particles);
}
