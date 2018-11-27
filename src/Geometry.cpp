#include "Geometry.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Geometry::Geometry() {
	drawMode = GL_TRIANGLES;
	vao = 0;
	vertexBuffer = 0;
	colourBuffer = 0;
	colourBuffer0 = 0;
	textureBuffer = 0;
	modelMatrix = glm::mat4(1.f);
	
/*	std::vector<glm::vec2> sp = {
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1024.0f), glm::vec2(1024.0f,0.0f),
		glm::vec2(1024.0f,0.0f), glm::vec2(0.0f,1024.0f), glm::vec2(1024.0f,1024.0f)};
	uvs = sp;*/
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
/*
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
*/
void Geometry::makeModel(std::vector<Face*> faces) {
	
//			std::vector<glm::vec3> facePoints;
	for (Face* f : faces) {
		std::vector<HalfEdge*> faceEdges;
		HalfEdge* curEdge = f->e;
		do {
	//				facePoints.push_back(curEdge->start->v);
			faceEdges.push_back(curEdge);
			curEdge = curEdge->nextEdge;
		}
		while (curEdge != f->e);
	//	for (glm::vec3 p : points) {
		int numEdges = faceEdges.size();
		// std::cout << numEdges << "\n";
		int r = (f->id & 0x000000FF) >> 0;
		int g = (f->id & 0x0000FF00) >> 8;
		int b = (f->id & 0x00FF0000) >> 16;
		if (numEdges == 4)
		{
			verts.push_back(faceEdges[0]->start->v);
			verts.push_back(faceEdges[1]->start->v);
			verts.push_back(faceEdges[2]->start->v);

			verts.push_back(faceEdges[3]->start->v);
			verts.push_back(faceEdges[0]->start->v);
			verts.push_back(faceEdges[2]->start->v);

			for (int i = 0; i < 6; i++)
			{
				// colours.push_back(glm::vec3(1.f, 0.f, 0.f));
				// colours1.push_back(glm::vec3(1.f, 0.f, 0.f));

				colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
				colours0.push_back(glm::vec3(0.5f));
				colours1.push_back(glm::vec3(0.5f));
//				colours1.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
//				selected.push_back(f->id);
//				selected0.push_back(f->id);
			}
			selected.push_back(f->id);
			selected.push_back(f->id);
			selected0.push_back(f->id);
			selected0.push_back(f->id);
		}
		else
		{
			 verts.push_back(faceEdges[0]->start->v);
			 verts.push_back(faceEdges[1]->start->v);
			 verts.push_back(faceEdges[2]->start->v);
			 for (int i = 0; i < 3; i++)
			 {
			// 	// colours.push_back(glm::vec3(1.f, 0.f, 0.f));
			// 	// colours1.push_back(glm::vec3(1.f, 0.f, 0.f));
			//
			 	colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
			 	colours0.push_back(glm::vec3(0.5f));
			 	colours1.push_back(glm::vec3(0.5f));
//			 	colours1.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
//			 	selected.push_back(f->id);
//			 	selected0.push_back(f->id);
			 }
			 selected.push_back(f->id);
			 selected0.push_back(f->id);
		}
		// for (HalfEdge* h : faceEdges) {
	// 		verts.push_back(h->start->v);
	// 		verts.push_back(h->nextEdge->start->v);
	// 		int r = (h->id & 0x000000FF) >> 0;
	// 		int g = (h->id & 0x0000FF00) >> 8;
	// 		int b = (h->id & 0x00FF0000) >> 16;
	// //		colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	// //		colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	// 		colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
	// 		colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
	// 		colours1.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
	// 		colours1.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
		// }
	}
	// glLineWidth(160.0f);
	drawMode = GL_TRIANGLES;
	
}
void Geometry::makeMesh(std::vector<Face*> faces) {
	
//			std::vector<glm::vec3> facePoints;
	for (Face* f : faces) {
		std::vector<HalfEdge*> faceEdges;
		HalfEdge* curEdge = f->e;
		do {
	//				facePoints.push_back(curEdge->start->v);
			faceEdges.push_back(curEdge);
			curEdge = curEdge->nextEdge;
		}
		while (curEdge != f->e);
		for (HalfEdge* h : faceEdges) {
	 		verts.push_back(h->start->v);
	 		verts.push_back(h->nextEdge->start->v);
			colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			colours.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			colours0.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			colours0.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			colours1.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			colours1.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			selected.push_back(f->id);
			selected0.push_back(f->id);
			
		 }
	}
	drawMode = GL_LINES;
	
}
void Geometry::highlightEdge(int eID) {
	int r = (eID & 0x000000FF) >> 0;
	int g = (eID & 0x0000FF00) >> 8;
	int b = (eID & 0x00FF0000) >> 16;
	glm::vec3 colID = glm::vec3(r/255.0f,g/255.0f,b/255.0f);
/*	int ra = ((eID+0x0000ff00) & 0x000000FF) >> 0;
	int ga = ((eID+0x0000ff00) & 0x0000FF00) >> 8;
	int ba = ((eID+0x0000ff00) & 0x00FF0000) >> 16;
	glm::vec3 colIDa = glm::vec3(ra/255.0f,ga/255.0f,ba/255.0f);
	*/
	for (int i=0; i<selected.size(); i++) {
		if (selected[i]==eID) {
			colours0[i*3] = glm::vec3(0.0f,0.0f,1.0f);
			colours0[i*3+1] = glm::vec3(0.0f,0.0f,1.0f);
			colours0[i*3+2] = glm::vec3(0.0f,0.0f,1.0f);
		}
	}
//	std::replace(colours.begin(), colours.end(), colID, colID+glm::vec3(0.0f,0.0f,1.0f));
}
void Geometry::unhighlightEdge() {

	colours0 = colours1;
	selected = selected0;
}
/*void Geometry::unhighlightEdge(int eID) {
	int r = (eID & 0x000000FF) >> 0;
	int g = (eID & 0x0000FF00) >> 8;
	int b = (eID & 0x00FF0000) >> 16;
	glm::vec3 colID = glm::vec3(r/255.0f,g/255.0f,b/255.0f);
	std::replace(colours.begin(), colours.end(), colID, colID-glm::vec3(0.0f,0.0f,1.0f));
}*/

void Geometry::makeFBO(std::vector<glm::vec3> screenPoints) {
//	for (glm::vec3 p : screenPoints)
	for (int i=0; i<6; i++) {
//		std::cout << screenPoints[i].x << "," << screenPoints[i].y << "," << screenPoints[i].z << std::endl;
		verts.push_back(screenPoints[i]);
	}
//		verts.push_back(p);
	drawMode = GL_TRIANGLES;
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
