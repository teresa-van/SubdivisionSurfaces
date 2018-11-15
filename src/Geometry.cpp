#include "Geometry.h"
#include <cmath>

Geometry::Geometry() {
	drawMode = GL_TRIANGLES;
	vao = 0;
	vertexBuffer = 0;
	colourBuffer = 0;
	modelMatrix = glm::mat4(1.f);
}

void Geometry::makeCube(glm::vec3 pos, float w, float h, float d) {
	glm::vec3 p0 = pos+glm::vec3(w,h,d);
	glm::vec3 p1 = pos+glm::vec3(-w,h,d);
	glm::vec3 p2 = pos+glm::vec3(w,-h,d);
	glm::vec3 p3 = pos+glm::vec3(-w,-h,d);
	glm::vec3 p4 = pos+glm::vec3(w,h,-d);
	glm::vec3 p5 = pos+glm::vec3(-w,h,-d);
	glm::vec3 p6 = pos+glm::vec3(w,-h,-d);
	glm::vec3 p7 = pos+glm::vec3(-w,-h,-d);
	
	verts.push_back(p0);
	verts.push_back(p1);
	verts.push_back(p2);
	verts.push_back(p3);
//	verts.push_back(p4);
//	verts.push_back(p5);
//	verts.push_back(p6);
//	verts.push_back(p7);
/*	verts.push_back(p0);
	verts.push_back(p2);
	verts.push_back(p4);
	verts.push_back(p6);
	verts.push_back(p1);
	verts.push_back(p3);
	verts.push_back(p5);
	verts.push_back(p7);
	verts.push_back(p0);
	verts.push_back(p4);
	verts.push_back(p1);
	verts.push_back(p5);
	verts.push_back(p2);
	verts.push_back(p6);
	verts.push_back(p3);
	verts.push_back(p7);*/
	
/*	verts.push_back(p0);
	verts.push_back(p1);
	verts.push_back(p2);
	verts.push_back(p1);
	verts.push_back(p3);
	verts.push_back(p2);
	
	verts.push_back(p4);
	verts.push_back(p5);
	verts.push_back(p6);
	verts.push_back(p5);
	verts.push_back(p7);
	verts.push_back(p6);
	
	verts.push_back(p1);
	verts.push_back(p5);
	verts.push_back(p3);
	verts.push_back(p5);
	verts.push_back(p7);
	verts.push_back(p3);
	
	verts.push_back(p0);
	verts.push_back(p4);
	verts.push_back(p2);
	verts.push_back(p4);
	verts.push_back(p6);
	verts.push_back(p2);
	
	verts.push_back(p0);
	verts.push_back(p1);
	verts.push_back(p4);
	verts.push_back(p1);
	verts.push_back(p5);
	verts.push_back(p4);
	
	verts.push_back(p2);
	verts.push_back(p3);
	verts.push_back(p6);
	verts.push_back(p3);
	verts.push_back(p7);
	verts.push_back(p6);
	*/
	for (int i=0; i<24; i++)
		colours.push_back(glm::vec3(0.0f,1.0f,0.0f));
	
//	drawMode = GL_TRIANGLES;
	drawMode = GL_LINE_LOOP;
//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

/*void Geometry::makeFace(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	verts.push_back(p0);
	verts.push_back(p1);
	verts.push_back(p2);
	verts.push_back(p3);
	for (int i=0; i<24; i++)
		colours.push_back(glm::vec3(1.0f,0.0f,0.0f));
	drawMode = GL_LINE_LOOP;
}*/
/*
void Geometry::makeFace(std::vector<glm::vec3> points) {
	for (glm::vec3 p : points) {
		verts.push_back(p);
		colours.push_back(glm::vec3(1.0f,0.0f,0.0f));
	}
	drawMode = GL_LINE_LOOP;
}*/
void Geometry::makeFace(Face* f) {
//			std::vector<glm::vec3> facePoints;
	std::vector<HalfEdge*> faceEdges;
	HalfEdge* curEdge = f->e;
	do {
//				facePoints.push_back(curEdge->start->v);
		faceEdges.push_back(curEdge);
		curEdge = curEdge->nextEdge;
	}
	while (curEdge != f->e);
//	for (glm::vec3 p : points) {
	for (HalfEdge* h : faceEdges) {
		verts.push_back(h->start->v);
		verts.push_back(h->nextEdge->start->v);
		int r = (h->id & 0x000000FF) >> 0;
		int g = (h->id & 0x0000FF00) >> 8;
		int b = (h->id & 0x00FF0000) >> 16;
//		colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
//		colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
		colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
//		colours.push_back(glm::vec3(1.0f,0.0f,0.0f));
	}
	glLineWidth(3.0f);
	drawMode = GL_LINES;
}

void Geometry::makePoint(glm::vec3 v) {
	verts.push_back(v);
	colours.push_back(glm::vec3(0.0f,0.0f,1.0f));
	drawMode = GL_POINTS;
}
/*
void Geometry::makeHEdge(HalfEdge *e) {
	verts.push_back(e->start);
	verts.push_back(e->nextEdge->start);
	colours.push_back(glm::vec3(0.0f,0.0f,1.0f));
	colours.push_back(glm::vec3(0.0f,0.0f,1.0f));
	drawMode = GL_LINES;
}
*/
//void Geometry::makeEdge(Vertex p0, Vertex p1) {
void Geometry::makeEdge(glm::vec3 p0, glm::vec3 p1) {
//	verts.push_back(glm::vec3(p0.x,p0.y,p0.z));
//	verts.push_back(glm::vec3(p1.x,p1.y,p1.z));
	verts.push_back(p0);
	verts.push_back(p1);
	colours.push_back(glm::vec3(1.0f,0.0f,0.0f));
	colours.push_back(glm::vec3(1.0f,0.0f,0.0f));
	drawMode = GL_LINES;
}

