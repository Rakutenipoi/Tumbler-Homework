#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

enum MESH_TYPE
{
	SPHERE,
	CUBE,

};

class StaticMesh {
public:
	unsigned vao, vbo, ebo;
	MESH_TYPE type;

	StaticMesh();
	virtual void generate();
	virtual void draw();
	void clear();
	void initBuffer();

protected:
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<unsigned int> indices;
};


class StaticSphere : public StaticMesh {
public:
	StaticSphere(float radius, int slices, int stacks);
	void generate() override;
	void draw() override;
	
private:
	float radius;
	int slices;
	int stacks;
};




