#include "Geometry.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Geometry::Geometry() {
	drawMode = GL_TRIANGLES;
	vao = 0;
	vao0 = 0;
	vertexBuffer = 0;
	colourBuffer = 0;
	colourBuffer0 = 0;
	textureBuffer = 0;
	vertexBuffer0 = 0;
	colourBuffer1 = 0;

	modelMatrix = glm::mat4(1.f);

/*	std::vector<glm::vec2> sp = {
		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1024.0f), glm::vec2(1024.0f,0.0f),
		glm::vec2(1024.0f,0.0f), glm::vec2(0.0f,1024.0f), glm::vec2(1024.0f,1024.0f)};
	uvs = sp;*/
}
void Geometry::clearGeometry() {
	verts.clear();
	verts0.clear();
	colours.clear();
	colours0.clear();
	colours1.clear();
	colours2.clear();
	selected.clear();
	selected0.clear();
//	modelMatrix = glm::mat4(1.f);
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
				colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
				colours0.push_back(glm::vec3(0.5f));
				colours1.push_back(glm::vec3(0.5f));
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
			 	colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
			 	colours0.push_back(glm::vec3(0.5f));
			 	colours1.push_back(glm::vec3(0.5f));
			 }
			 selected.push_back(f->id);
			 selected0.push_back(f->id);
		}
		 for (HalfEdge* h : faceEdges) {
	 		verts0.push_back(h->start->v);
	 		verts0.push_back(h->nextEdge->start->v);
			colours2.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			colours2.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		 }
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

void Geometry::elevateFace(int fID, float height) {
	Face* f = EdgeIDs[fID];
	std::vector<HalfEdge*> faceEdges;
	HalfEdge* curEdge = f->e;
	do {
//				facePoints.push_back(curEdge->start->v);
		faceEdges.push_back(curEdge);
		curEdge = curEdge->nextEdge;
	}
	while (curEdge != f->e);
	glm::vec3 direction = glm::normalize(glm::cross(faceEdges[0]->start->v-faceEdges[1]->start->v,faceEdges[2]->start->v-faceEdges[1]->start->v));

	int numEdges = faceEdges.size();
	// std::cout << numEdges << "\n";
	int r = (f->id & 0x000000FF) >> 0;
	int g = (f->id & 0x0000FF00) >> 8;
	int b = (f->id & 0x00FF0000) >> 16;

	if (EdgeIDs[fID]->elevation+height != 0 && height != 0) {

		EdgeIDs[fID]->elevation += height;
//		Face* f = EdgeIDs[fID];
//		std::vector<HalfEdge*> faceEdges;
		std::vector<Vertex*> newVertices;
		std::vector<Face*> newFaces;
		std::vector<HalfEdge*> newHEdgesNewFace;
		std::vector<HalfEdge*> newHEdgesOldFace;
		for (HalfEdge* h : faceEdges) {
			Vertex* newvert= new Vertex();
			newvert->v = h->nextEdge->start->v + height*direction;
			newvert->e = h;
			newVertices.push_back(newvert);
			Face* newFace= new Face();
			newFace->id = f->id+100;
//			newFace->id = InputHandler::idCounter;
			newFace->elevation = 0;
			newFace->e = h;
//			InputHandler::idCounter++;
			newFaces.push_back(newFace);
			HalfEdge* newhe0 = new HalfEdge();
			HalfEdge* newhe1 = new HalfEdge();
			HalfEdge* newhe2 = new HalfEdge();
			HalfEdge* newhe3 = new HalfEdge();

			newHEdgesNewFace.push_back(h->nextEdge);
			newHEdgesNewFace.push_back(newhe0);
			newHEdgesNewFace.push_back(newhe1);
			newHEdgesNewFace.push_back(newhe2);
			newHEdgesOldFace.push_back(newhe3);
		}

		for (int i=0; i<newHEdgesOldFace.size(); i++) {
			if (i == newHEdgesOldFace.size()-1) {
				newHEdgesOldFace[i]->nextEdge = newHEdgesOldFace[0];
			}
			else
				newHEdgesOldFace[i]->nextEdge = newHEdgesOldFace[i];
			newHEdgesOldFace[i]->start = newVertices[i];
			newHEdgesOldFace[i]->f = EdgeIDs[fID];

			newHEdgesOldFace[i]->pairEdge = newHEdgesNewFace[i*4+2];
			if (i == newHEdgesOldFace.size()-1)
				newHEdgesOldFace[i]->pairEdge->start = newVertices[0];
			else
				newHEdgesOldFace[i]->pairEdge->start = newVertices[i+1];
			newHEdgesOldFace[i]->pairEdge->nextEdge = newHEdgesNewFace[i*4+3];
			newHEdgesOldFace[i]->pairEdge->nextEdge->start = newVertices[i];
			newHEdgesOldFace[i]->pairEdge->nextEdge->nextEdge = newHEdgesNewFace[i*4];
			newHEdgesOldFace[i]->pairEdge->nextEdge->nextEdge->nextEdge = newHEdgesNewFace[i*4+1];
			newHEdgesOldFace[i]->pairEdge->nextEdge->nextEdge->nextEdge->start = newHEdgesNewFace[i*4]->pairEdge->start;
			newHEdgesOldFace[i]->pairEdge->nextEdge->nextEdge->nextEdge->nextEdge = newHEdgesNewFace[i*4+2];

//			newHEdgesNewFace[i*4+2]->start = newVertices[i*4];
//			newHEdgesNewFace[i*4+3]->start = newVertices[i];

			if (i == 0)
				newHEdgesNewFace[i*4+3] = newHEdgesNewFace[(newHEdgesOldFace.size()-1)*4+1];
			else
				newHEdgesNewFace[i*4+3] = newHEdgesNewFace[(i-1)*4+1];
			if (i == newHEdgesOldFace.size()-1)
				newHEdgesNewFace[i*4+1] = newHEdgesNewFace[3];
			else
				newHEdgesNewFace[i*4+1] = newHEdgesNewFace[(i+1)*4+1];
		}

		for (Face* fa : newFaces) {
			std::vector<HalfEdge*> faceEdges0;
			HalfEdge* curEdge = fa->e;
			do {
		//				facePoints.push_back(curEdge->start->v);
				faceEdges0.push_back(curEdge);
				curEdge = curEdge->nextEdge;
			}
			while (curEdge != fa->e);

/*				std::cout<< faceEdges0[0]->start->v.x<<","<< faceEdges0[0]->start->v.y<<","<< faceEdges0[0]->start->v.z << std::endl;
				std::cout<< faceEdges0[1]->start->v.x<<","<< faceEdges0[1]->start->v.y<<","<< faceEdges0[1]->start->v.z << std::endl;
				std::cout<< faceEdges0[2]->start->v.x<<","<< faceEdges0[2]->start->v.y<<","<< faceEdges0[2]->start->v.z << std::endl;
				std::cout<< "-----" << std::endl;
				std::cout<< faceEdges0[3]->start->v.x<<","<< faceEdges0[3]->start->v.y<<","<< faceEdges0[3]->start->v.z << std::endl;
				std::cout<< faceEdges0[0]->start->v.x<<","<< faceEdges0[0]->start->v.y<<","<< faceEdges0[0]->start->v.z << std::endl;
				std::cout<< faceEdges0[2]->start->v.x<<","<< faceEdges0[2]->start->v.y<<","<< faceEdges0[2]->start->v.z << std::endl;
				std::cout<< "-----" << std::endl;
				std::cout<< "-----" << std::endl;
				*/
				verts.push_back(faceEdges0[0]->start->v);
				verts.push_back(faceEdges0[1]->start->v);
				verts.push_back(faceEdges0[2]->start->v);

				verts.push_back(faceEdges0[3]->start->v);
				verts.push_back(faceEdges0[0]->start->v);
				verts.push_back(faceEdges0[2]->start->v);

				for (int i = 0; i < 6; i++)
				{
					colours.push_back(glm::vec3(r/255.0f,g/255.0f,b/255.0f));
					colours0.push_back(glm::vec3(0.0f));
					colours1.push_back(glm::vec3(0.50f));
				}
				selected.push_back(fa->id);
				selected.push_back(fa->id);
				selected0.push_back(fa->id);
				selected0.push_back(fa->id);

			 for (HalfEdge* h : faceEdges0) {
				verts0.push_back(h->start->v);
				verts0.push_back(h->nextEdge->start->v);
				colours2.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				colours2.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			 }
		}

			if (numEdges == 4)
			{
				for (int i=0; i<selected.size(); i++) {
					if (selected[i]==fID) {
						verts[i*3] = newVertices[0]->v;
						verts[i*3+1] = newVertices[1]->v;
						verts[i*3+2] = newVertices[2]->v;
						verts[i*3+3] = newVertices[3]->v;
						verts[i*3+4] = newVertices[0]->v;
						verts[i*3+5] = newVertices[2]->v;
						i+=6;
					}
				}

			}
			else
			{
				for (int i=0; i<selected.size(); i++) {
					if (selected[i]==fID) {
						verts[i*3] = newVertices[0]->v;
						verts[i*3+1] = newVertices[1]->v;
						verts[i*3+2] = newVertices[2]->v;
						i+=3;
					}
				}
			}

	}
	else if (EdgeIDs[fID]->elevation != 0 && EdgeIDs[fID]->elevation+height == 0) {
		EdgeIDs[fID]->elevation += height;

//		Face* f = EdgeIDs[fID];
//		std::vector<HalfEdge*> faceEdges;
		std::vector<Face*> faceToDelete;
		std::vector<Vertex*> vertToDelete;
		std::vector<HalfEdge*> HEToDelete;
//		HalfEdge* curEdge = f->e;
//		do {
	//				facePoints.push_back(curEdge->start->v);
//			faceEdges.push_back(curEdge);
//			curEdge = curEdge->nextEdge;
//		}
//		while (curEdge != f->e);
//		glm::vec3 direction = glm::normalize(glm::cross(faceEdges[0]->start->v-faceEdges[1]->start->v,faceEdges[2]->start->v-faceEdges[1]->start->v));
		for (HalfEdge* h : faceEdges) {
			vertToDelete.push_back(h->start);
			h = h->pairEdge->nextEdge->nextEdge;
//			h->start->v = h->pairEdge->nextEdge->nextEdge->start->v;
			faceToDelete.push_back(h->pairEdge->f);
			HEToDelete.push_back(h->pairEdge->nextEdge);
			HEToDelete.push_back(h->pairEdge->nextEdge->nextEdge);
			HEToDelete.push_back(h->pairEdge->nextEdge->nextEdge->nextEdge);
			HEToDelete.push_back(h->pairEdge->nextEdge->nextEdge->nextEdge->nextEdge);
		}
		for(HalfEdge* e : HEToDelete)
			delete e;
		for (Vertex* v : vertToDelete)
			delete v;
		for (Face* f : faceToDelete)
			delete f;
	}
//	else {
/*		std::vector<glm::vec3> vs;
		for (HalfEdge* h : faceEdges) {
			h->start->v = h->start->v + height*direction;
			vs.push_back(h->start->v + height*direction);
		}
		if (numEdges == 4)
		{
			for (int i=0; i<selected.size(); i++) {
				if (selected[i]==fID) {
					verts[i*3] = vs[0];
					verts[i*3+1] = vs[1];
					verts[i*3+2] = vs[2];
					verts[i*3+3] = vs[3];
					verts[i*3+4] = vs[0];
					verts[i*3+5] = vs[2];
					i+=6;
				}
			}
		}
		else {
			for (int i=0; i<selected.size(); i++) {
				if (selected[i]==fID) {
					verts[i*3] = vs[0];
					verts[i*3+1] = vs[1];
					verts[i*3+2] = vs[2];
					i+=3;
				}
			}
		}
	}
	*/


//	 		verts0.push_back(h->start->v);
//	 		verts0.push_back(h->nextEdge->start->v);
	//	for (glm::vec3 p : points) {
//		int numEdges = faceEdges.size();

}

void Geometry::subdivideMesh(Mesh * mesh)
{
	// CHECK SCOPES
//	std::vector<Face*> faces = mesh->faces;	// all faces on mesh, might have to be global
//	std::vector<Vertex*> vertices = mesh->vertices; // all vertices on mesh, might have to be global
	std::vector<Face*> newFaces;
	std::vector<Vertex*> FVs;
	std::vector<Vertex*> EVs;

	//Creates a lsit of new vertices per face
	for (Face* f : mesh->faces)
	{
		// std::cout << "1. Face: " << f->id << "\n";

		Vertex* v0 = new Vertex();
		HalfEdge* current = f->e;
		v0->v = glm::vec3(0.0f);
		float nEdges = 0.0f;

		do
		{
			v0->v += current->start->v;
			nEdges++;
			current = current->nextEdge;
		} while (current!= f->e);

		v0->v = v0->v/nEdges;

//		v0->v = (current->start->v + current->nextEdge->nextEdge->start->v)/2.0f;
		FVs.push_back(v0);

		// std::cout << current->f->id << " <- current \n";
		// std::cout << current->pairEdge->f->id << " <- pair \n";
		// HalfEdge* c = current;
		//
		// do
		// {
		// 	std::cout << c->f->id << " <- next \n";
		// 	c = c->nextEdge;
		// } while(c != f->e);
		//
		// std::cout << "\n";
	}

	// std::cout << "\n";

//	for (Vertex* v0 : FVs)
//		std::cout << v0->v.x << "," << v0->v.y << "," << v0->v.z << std::endl;

	//Creates list of new vertices per edge
	for (Face* f : mesh->faces)
	{
//		std::cout << "\n2. Face: " << f->id << "\n";

		glm::vec3 p = glm::vec3(0.0f);
		HalfEdge* current = f->e;
		do
		{
//			p = (current->start->v + FVs[current->f->id]->v + current->pairEdge->start->v + FVs[current->pairEdge->f->id]->v) / 4.0f;

			// checks if the edge has already been split
//					std::cout << p.x << "," << p.y << "," << p.z << std::endl;
//					std::cout << std::endl;
/*			if (p == current->pairEdge->start->v)
			{
				 std::cout << "here\n";
				current = current->nextEdge->nextEdge;
			}*/
			// std::cout << current->f->id << " <- face id \n";

			if (find(EVs.begin(), EVs.end(), current->nextEdge->start) != EVs.end())
			{
				current = current->nextEdge->nextEdge;
			}
			else
			{
				p = (current->start->v + FVs[current->f->id]->v + current->pairEdge->start->v + FVs[current->pairEdge->f->id]->v) / 4.0f;
//				p = (current->start->v + current->nextEdge->start->v)/2.0f;
				Vertex* v0 = new Vertex();
				v0->v = p;

				HalfEdge* HEnext = new HalfEdge();
				HalfEdge* HEpair = new HalfEdge();

				// updates edges next edge and pair edge
				// for now, every face will have twice as many vertics it started with
				HEnext->nextEdge = current->nextEdge;
				HEnext->pairEdge = current->pairEdge;
				HEnext->start = v0;

				HEpair->nextEdge = current->pairEdge->nextEdge;
				HEpair->pairEdge = current;
				HEpair->start = v0;

				// current->pairEdge->pairEdge = HEnext;
				// current->pairEdge->nextEdge = HEpair;

				HEnext->pairEdge->pairEdge = HEnext;
				HEnext->pairEdge->nextEdge = HEpair;

				HEnext->f = f;
				HEpair->f = current->pairEdge->f;

				current->nextEdge = HEnext;
				current->pairEdge = HEpair;

				v0->e = HEnext;
				EVs.push_back(v0);
				// current = HEnext->nextEdge;
				current = current->nextEdge->nextEdge;
			}
		} while (current != f->e);
	}
	std::cout << "SPLITING EDGES COMPLETE" << std::endl;
//	std::cout << mesh->idCounter << std::endl;
	// now we add edges and faces so every face now has 4 faces
	for (Face* f:mesh->faces)
	{
//		std::cout << "Face: " << f->id << "\n";

		HalfEdge* current = f->e;
		HalfEdge* prevE;
		HalfEdge* lastE;
		// gets the edge that connects to the start edge
		do
		{
//			std::cout << current->f->id << std::endl;
			prevE = current;
			current = current->nextEdge;
		} while (current!= f->e);
		current = f->e;
		do
		{
//			std::cout << current->f->id << "\n";

			HalfEdge* HE1 = new HalfEdge();
			HalfEdge* HE2 = new HalfEdge();
			HE1->nextEdge = HE2;
			HE2->nextEdge = prevE;
			HE1->start = current->nextEdge->start;
			HE2->start = FVs[current->f->id];
			FVs[current->f->id]->e = HE2;

			if (current != f->e) {
				HE2->pairEdge=lastE;
				lastE->pairEdge=HE2;
				Face* nf = new Face();	// creates n-1 new faces
				current->f = nf;
				HE1->f = nf;
				HE2->f = nf;
				prevE->f = nf;
//				nf->id = 0; // NEEDS UNIQUE ID
				nf->e = current;
//				nf->elevation = 0;
				nf->id = mesh->idCounter; // NEEDS UNIQUE ID
				mesh->idCounter++;

				newFaces.push_back(nf);
//				std::cout << newFaces.size() << std::endl;
			}
			else
			{ // first face is the orignal face with new values

				HE1->f = f;
				HE2->f = f;

//				std::cout << "fasdfadsfasdf" << std::endl;
				prevE->f = f;
			}

			prevE = current->nextEdge;
			current->nextEdge = HE1;
			current = prevE->nextEdge;
			lastE = HE1;
		} while (current != f->e);// && counter < nEdges);
//	std::cout << "here\n";
		f->e->nextEdge->nextEdge->pairEdge = lastE;
		lastE = f->e->nextEdge->nextEdge->pairEdge;
	}

	std::cout << "FACES COMPLETE" << std::endl;


//	std::cout << vertices.size() << std::endl;
	// changes values of vertices (excluding new EVs and FVs)
	// std::cout << mesh->vertices.size() << std::endl;
	for (Vertex* v0 : mesh->vertices) {
		float nADJ = 0.0f;
		glm::vec3 ve = glm::vec3(0.0f);
		glm::vec3 vf = glm::vec3(0.0f);
		HalfEdge* current = v0->e; // MIGHT NOT HAVE ASSIGNED THIS IN PROGRAM.CPP AFTER OBJLOADING
		do {
			// v0->v += FVs[current->f->id]->v;
//			v0->v += current->pairEdge->start->v;
//			v0->v += current->nextEdge->nextEdge->start->v;
//			v0->v += current->pairEdge->start->v;
			ve+=current->pairEdge->start->v;
			vf+=current->nextEdge->nextEdge->start->v;
			current = current->pairEdge->nextEdge;
			nADJ++;
		} while (current != v0->e);
		v0->v = (nADJ-2)*v0->v/nADJ+ve/(nADJ*nADJ)+vf/(nADJ*nADJ);
/*		do {
			// v0->v += FVs[current->f->id]->v;
//			v0->v += current->pairEdge->start->v;
			v0->v += current->nextEdge->nextEdge->start->v;
			v0->v += current->pairEdge->start->v;
			current = current->pairEdge->nextEdge;
			nADJ+=2;
		} while (current != v0->e);*/
//		v0->v = v0->v/nADJ;
	}


	// add the new vertices to the lsit of vertices
	for (Vertex* v0 : FVs)
		mesh->vertices.push_back(v0);
	for (Vertex* v0 : EVs)
		mesh->vertices.push_back(v0);
	for (Face* f0 : newFaces)
		mesh->faces.push_back(f0);


/*	for (Face* f : mesh->faces) {
		HalfEdge* current = f->e;
		std::cout << f->id << std::endl;
		std::cout << "-----------------------" << std::endl;
		do {
			std::cout << current->f->id << std::endl;
			current = current->nextEdge;
		} while (current != f->e);
		std::cout << std::endl;
		std::cout << std::endl;
	}
*/

	this->clearGeometry();
	this->makeModel(mesh->faces);

	std::cout << "SUBDIVISION COMPLETE" << std::endl;
}

void Geometry::subdivideFaces(Mesh * mesh, std::vector<int> pickedIDs)
{
	// CHECK SCOPES
	std::vector<Face*> newFaces;
	std::vector<Vertex*> FVs;
	std::vector<Vertex*> EVs;

	std::vector<Vertex*> selectedFaceVerts;
	std::vector<Face*> selectedFaces;

	for (Face* f : mesh->faces)
	{
		if (find(pickedIDs.begin(), pickedIDs.end(), f->id) != pickedIDs.end())
			selectedFaces.push_back(f);
	}

	//Creates a lsit of new vertices per face
	for (Face* f : selectedFaces)
	{
		Vertex* v0 = new Vertex();
		HalfEdge* current = f->e;
		v0->v = glm::vec3(0.0f);
		float nEdges = 0.0f;

		do
		{
			v0->v += current->start->v;
			nEdges++;

			selectedFaceVerts.push_back(current->start);

			current = current->nextEdge;
		} while (current!= f->e);

		v0->v = v0->v/nEdges;

		FVs.push_back(v0);
	}

	int counter = 0;
	//Creates list of new vertices per edge
	for (Face* f : selectedFaces)
	{
		glm::vec3 p = glm::vec3(0.0f);
		HalfEdge* current = f->e;
		do
		{
			if (find(EVs.begin(), EVs.end(), current->nextEdge->start) != EVs.end())
			{
				current = current->nextEdge->nextEdge;
			}
			else
			{

				glm::vec3 fvsOpp = glm::vec3(0.0f);
				HalfEdge* c = current->pairEdge;
				float x = 0.0f;
				do
				{
					fvsOpp += c->start->v;
					x++;
					c = c->nextEdge;
				} while (c!= current->pairEdge);
				fvsOpp = fvsOpp / x;
				// std::cout << fvsOpp.x << "\n";

				p = (current->start->v + FVs[counter]->v + current->pairEdge->start->v + fvsOpp) / 4.0f;
				// p = (current->start->v + FVs[current->f->id]->v + current->pairEdge->start->v + FVs[current->pairEdge->f->id]->v) / 4.0f;
				// p = (current->start->v + current->pairEdge->start->v) / 2.0f;

				Vertex* v0 = new Vertex();
				v0->v = p;

				HalfEdge* HEnext = new HalfEdge();
				HalfEdge* HEpair = new HalfEdge();

				// updates edges next edge and pair edge
				// for now, every face will have twice as many vertics it started with
				HEnext->nextEdge = current->nextEdge;
				HEnext->pairEdge = current->pairEdge;
				HEnext->start = v0;

				HEpair->nextEdge = current->pairEdge->nextEdge;
				HEpair->pairEdge = current;
				HEpair->start = v0;

				HEnext->pairEdge->pairEdge = HEnext;
				HEnext->pairEdge->nextEdge = HEpair;

				HEnext->f = f;
				HEpair->f = current->pairEdge->f;

				current->nextEdge = HEnext;
				current->pairEdge = HEpair;

				v0->e = HEnext;
				EVs.push_back(v0);
				current = current->nextEdge->nextEdge;
			}
		} while (current != f->e);
	}

	std::cout << "SPLITING EDGES COMPLETE" << std::endl;
	// now we add edges and faces so every face now has 4 faces
	counter = 0;
	for (Face* f : selectedFaces)
	{
		HalfEdge* current = f->e;
		HalfEdge* prevE;
		HalfEdge* lastE;
		// gets the edge that connects to the start edge
		do
		{
			prevE = current;
			current = current->nextEdge;
		} while (current!= f->e);
		current = f->e;
		do
		{
			HalfEdge* HE1 = new HalfEdge();
			HalfEdge* HE2 = new HalfEdge();

			HE1->nextEdge = HE2;
			HE2->nextEdge = prevE;
			HE1->start = current->nextEdge->start;
			HE2->start = FVs[counter];
			FVs[counter]->e = HE2;

			if (current != f->e) {
				HE2->pairEdge=lastE;
				lastE->pairEdge=HE2;
				Face* nf = new Face();	// creates n-1 new faces
				current->f = nf;
				HE1->f = nf;
				HE2->f = nf;
				prevE->f = nf;
				nf->e = current;
				nf->id = mesh->idCounter; // NEEDS UNIQUE ID
				mesh->idCounter++;

				newFaces.push_back(nf);
			}
			else
			{ // first face is the orignal face with new values
				HE1->f = f;
				HE2->f = f;
				prevE->f = f;
			}

			prevE = current->nextEdge;
			current->nextEdge = HE1;
			current = prevE->nextEdge;
			lastE = HE1;
		} while (current != f->e);// && counter < nEdges);
		f->e->nextEdge->nextEdge->pairEdge = lastE;
		lastE = f->e->nextEdge->nextEdge->pairEdge;
		counter++;
	}

	std::cout << "FACES COMPLETE" << std::endl;
	// changes values of vertices (excluding new EVs and FVs)

	for (Vertex* v0 : selectedFaceVerts)
	{
		float nADJ = 0.0f;
		glm::vec3 ve = glm::vec3(0.0f);
		glm::vec3 vf = glm::vec3(0.0f);


		HalfEdge* current = v0->e; // MIGHT NOT HAVE ASSIGNED THIS IN PROGRAM.CPP AFTER OBJLOADING
		do
		{
			// std::cout << v0->e->f->id << "\n";

			ve+=current->pairEdge->start->v;
			vf+=current->nextEdge->nextEdge->start->v;
			current = current->pairEdge->nextEdge;
			nADJ++;
		} while (current != v0->e);

		v0->v = (nADJ-2)*v0->v/nADJ+ve/(nADJ*nADJ)+vf/(nADJ*nADJ);

	}

	// add the new vertices to the lsit of vertices
	for (Vertex* v0 : FVs)
		mesh->vertices.push_back(v0);
	for (Vertex* v0 : EVs)
		mesh->vertices.push_back(v0);
	for (Face* f0 : newFaces)
		mesh->faces.push_back(f0);

	// std::cout << mesh->vertices.size() << " <- # of vertices\n";

	this->clearGeometry();
	this->makeModel(mesh->faces);

	std::cout << "SUBDIVISION COMPLETE" << std::endl;
}
