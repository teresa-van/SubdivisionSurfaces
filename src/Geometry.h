#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>

#include "myobjects.h"

//#include "InputHandler.h"

struct HalfEdge;
struct Vertex;
struct Face;
struct Edge;

struct HalfEdge {
	HalfEdge* nextEdge;
	HalfEdge* pairEdge;
	Vertex* start;
	Face* f;
	float sharpness;
};

struct Vertex {
//	float x, y, z;
	glm::vec3 v;
	glm::vec3 vn;
	HalfEdge* e;
};

struct Face {
	HalfEdge* e;
	int id;
	int elevation;
	Vertex* center;
};

struct Mesh
{
	std::vector<Face*> faces;
	std::vector<Vertex*> vertices;
	int idCounter;
};


class Geometry {

public:
	Geometry();

	GLuint drawMode;

	GLuint vao;
	GLuint vao0;
	GLuint vertexBuffer;
	GLuint colourBuffer;
	GLuint textureBuffer;
	GLuint vertexBuffer0;
	GLuint colourBuffer0;
	GLuint colourBuffer1;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> colours;
	std::vector<glm::vec2> uvs;

	std::vector<glm::vec3> verts0;
	std::vector<glm::vec3> colours0;
	std::vector<glm::vec3> colours1;
	std::vector<glm::vec3> colours2;
	glm::mat4 modelMatrix;

	void makeModel(std::vector<Face*> faces);
	void clearGeometry();

	void highlightEdge(int eID);
	void unhighlightEdge();
	void makeFBO(std::vector<glm::vec3> screenPoints);

	static std::map<int, Face*> EdgeIDs;
	std::vector<int> selected;
	std::vector<int> selected0;

	void subdivideMesh(Mesh* mesh);
	void subdivideFaces(Mesh* mesh, std::vector<int> *pickedIDs);
	
	void pullFace(Mesh* mesh, std::vector<int> *pickedIDs);
	void writeToOBJ(Mesh* mesh, std::string path);
	void readFromOBJ(Mesh* mesh, char* path);
};
