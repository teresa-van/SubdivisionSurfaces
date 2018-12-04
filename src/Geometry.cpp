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

void Geometry::subdivideMesh() { 
	// CHECK SCOPES
	std::vector<Face*> faces;	// all faces on mesh, might have to be global
	std::vector<Vertex*> vertices; // all vertices on mesh, might have to be global
	std::vector<Face*> newFaces;
	std::vector<Vertex*> FVs;
	std::vector<Vertex*> EVs;
	
	//Creates a lsit of new vertices per face
	for (Face* f:faces) {
		Vertex* v0 = new Vertex();
		HalfEdge* current = f->e;
		v0->v = glm::vec3(0.0f);
		float nEdges = 0.0f;
		do {
			v0->v += current->start->v;
			nEdges++;
		} while (current!= f->e);
		v0->v = v0->v/nEdges;
		FVs.push_back(v0);
	}
	//Creates list of new vertices per edge
	for (Face* f:faces) {
		glm::vec3 p = glm::vec3(0.0f);
		HalfEdge* current = f->e;
		do {
			p = (current->start->v + FVs[current->f->id]->v +current->pairEdge->start->v + FVs[current->pairEdge->f->id]->v)/4.0f;
			// checks if the edge has already been split
			if (p == current->pairEdge->start->v) 
				current = current->nextEdge->nextEdge;
			else {
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
				current->pairEdge->pairEdge = HEnext;
				current->pairEdge->nextEdge = HEpair;
				EVs.push_back(v0);
				current = HEnext->nextEdge;
			}
		} while (current != f->e);
	}
	// now we add edges and faces so every face now has 4 faces
	for (Face* f:faces) {
		HalfEdge* current = f->e;
		HalfEdge* prevE;
		HalfEdge* lastE;
		// gets the edge that connects to the start edge
		do {
			lastE = current;
			current = current->nextEdge;
		}
		while (current!= f->e);
		do {
			HalfEdge* HE1 = new HalfEdge();
			HalfEdge* HE2 = new HalfEdge();
			HE1->nextEdge = HE2;
			HE2->nextEdge = prevE;
			if (current != f->e) {
				HE2->pairEdge=lastE;
				lastE->pairEdge=HE2;
				Face* nf = new Face();	// creates n-1 new faces
				current->f = nf;
				HE1->f = nf;
				HE2->f = nf;
				prevE->f = nf;
				nf->id = 0; // NEEDS UNIQUE ID
				newFaces.push_back(nf);
			}
			else { // first face is the orignal face with new values
				HE1->f = f;
				HE2->f = f;
				prevE->f = f;
			}
			prevE = current->nextEdge;
			current->nextEdge = HE1;
			current = prevE->nextEdge;
			lastE = HE1;
		} while (current != f->e);
		f->e->nextEdge->nextEdge->pairEdge = lastE;
		lastE = f->e->nextEdge->nextEdge->pairEdge;
	}
	// changes values of vertices (excluding new EVs and FVs)
	for (Vertex* v0 : vertices) {
		float nADJ = 1.0f;
		HalfEdge* current = v0->e; // MIGHT NOT HAVE ASSIGNED THIS IN PROGRAM.CPP AFTER OBJLOADING
		do {
			v0->v += current->pairEdge->start->v;
			current = current->pairEdge->nextEdge;
			nADJ++;
		} while (current != v0->e);
		v0->v = v0->v/nADJ;
	}
	// add the new vertices to the lsit of vertices
	for (Vertex* v0 : FVs) 
		vertices.push_back(v0);	
	for (Vertex* v0 : EVs) 
		vertices.push_back(v0);
	for (Face* f0 : newFaces)
		faces.push_back(f0);
		
}

