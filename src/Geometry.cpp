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
}

void Geometry::makeModel(std::vector<Face*> faces) {
	for (Face* f : faces) {
		std::vector<HalfEdge*> faceEdges;
		HalfEdge* curEdge = f->e;
		do {
			faceEdges.push_back(curEdge);
			curEdge = curEdge->nextEdge;
		}
		while (curEdge != f->e);
		int numEdges = faceEdges.size();
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
	drawMode = GL_TRIANGLES;
}
/*void Geometry::makeMesh(std::vector<Face*> faces) {

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

}*/
void Geometry::highlightEdge(int eID) {
	int r = (eID & 0x000000FF) >> 0;
	int g = (eID & 0x0000FF00) >> 8;
	int b = (eID & 0x00FF0000) >> 16;
	glm::vec3 colID = glm::vec3(r/255.0f,g/255.0f,b/255.0f);
	for (int i=0; i<selected.size(); i++) {
		if (selected[i]==eID) {
			colours0[i*3] = glm::vec3(0.0f,0.0f,1.0f);
			colours0[i*3+1] = glm::vec3(0.0f,0.0f,1.0f);
			colours0[i*3+2] = glm::vec3(0.0f,0.0f,1.0f);
		}
	}
}

void Geometry::unhighlightEdge() {

	colours0 = colours1;
	selected = selected0;
}

void Geometry::makeFBO(std::vector<glm::vec3> screenPoints) {
	for (int i=0; i<6; i++) {
		verts.push_back(screenPoints[i]);
	}
	drawMode = GL_TRIANGLES;
}

void Geometry::subdivideMesh(Mesh * mesh)
{
	std::vector<Face*> newFaces;
	std::vector<Vertex*> FVs;
	std::vector<Vertex*> EVs;

	//Creates list of new vertices per edge
	for (Face* f : mesh->faces)
	{
		FVs.push_back(f->center);

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
				p = (current->start->v + current->f->center->v + current->pairEdge->start->v + current->pairEdge->f->center->v) / 4.0f;
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
				// current = HEnext->nextEdge;
				current = current->nextEdge->nextEdge;
			}
		} while (current != f->e);
	}
	std::cout << "SPLITING EDGES COMPLETE" << std::endl;

	// now we add edges and faces so every face now has 4 faces
	for (Face* f:mesh->faces)
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
			HE2->start = current->f->center;

			current->f->center->e = HE2;

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
	}

	std::cout << "FACES COMPLETE" << std::endl;

	for (Vertex* v0 : mesh->vertices) {
		float nADJ = 0.0f;
		glm::vec3 ve = glm::vec3(0.0f);
		glm::vec3 vf = glm::vec3(0.0f);
		HalfEdge* current = v0->e; // MIGHT NOT HAVE ASSIGNED THIS IN PROGRAM.CPP AFTER OBJLOADING
		do
		{
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

	//Creates a lsit of new vertices per face
	for (Face* f : mesh->faces)
	{
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

		f->center = v0;
		f->center->vn = glm::normalize(glm::cross(
			f->e->nextEdge->start->v - f->e->start->v,
			f->e->nextEdge->nextEdge->start->v - f->e->start->v));
	}

	this->clearGeometry();
	this->makeModel(mesh->faces);

	std::cout << "SUBDIVISION COMPLETE" << std::endl;
}

void Geometry::subdivideFaces(Mesh * mesh, std::vector<int> *pickedIDs)
{
	// CHECK SCOPES
	std::vector<Face*> newFaces;
	std::vector<Vertex*> FVs;
	std::vector<Vertex*> EVs;

	std::vector<Vertex*> selectedFaceVerts;
	std::vector<Face*> selectedFaces;
	std::vector<Face*> ringFaces;
	std::vector<Face*> unselectedAdjFaces;

	for (Face* f : mesh->faces)
	{
		if (find(pickedIDs->begin(), pickedIDs->end(), f->id) != pickedIDs->end())
		{
			selectedFaces.push_back(f);
			HalfEdge* current = f->e;
			do
			{
				if (find(selectedFaceVerts.begin(), selectedFaceVerts.end(), current->start) == selectedFaceVerts.end())
					selectedFaceVerts.push_back(current->start);
				current = current->nextEdge;
			} while (current!= f->e);
		}
	}
	
	for (Face* f : selectedFaces) {
		ringFaces.push_back(f);
		HalfEdge* current = f->e;
		do
		{
			if (find(ringFaces.begin(), ringFaces.end(), current->pairEdge->f) == ringFaces.end()
					&& find(selectedFaces.begin(), selectedFaces.end(), current->pairEdge->f) == selectedFaces.end())
					ringFaces.push_back(current->pairEdge->f);
			current = current->nextEdge;
		} while (current != f->e);
	}

	//Creates list of new vertices per edge
	for (Face* f : ringFaces)
	{
		FVs.push_back(f->center);

		glm::vec3 p = glm::vec3(0.0f);
		HalfEdge* current = f->e;
		do
		{
			if (find(unselectedAdjFaces.begin(), unselectedAdjFaces.end(), current->pairEdge->f) == unselectedAdjFaces.end()
					&& find(selectedFaces.begin(), selectedFaces.end(), current->pairEdge->f) == selectedFaces.end())
					unselectedAdjFaces.push_back(current->pairEdge->f);

			if (find(EVs.begin(), EVs.end(), current->nextEdge->start) != EVs.end())
			{
				current = current->nextEdge->nextEdge;
			}
			else
			{
				p = (current->start->v + current->f->center->v + current->pairEdge->start->v + current->pairEdge->f->center->v) / 4.0f;
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
				current = current->nextEdge->nextEdge;
			}
		} while (current != f->e);
	}

	std::cout << "SPLITING EDGES COMPLETE" << std::endl;
	// now we add edges and faces so every face now has 4 faces
	for (Face* f:selectedFaces)
	{
		int nEdges = 0;
		HalfEdge* current = f->e;
		do
		{
			current = current->nextEdge;
			nEdges++;
		} while(current != f->e);

		current = f->e;
		while (find(EVs.begin(), EVs.end(), current->start) == EVs.end())
			current = current->nextEdge;

		if (nEdges == 8)
		{
			HalfEdge* HE1 = current->nextEdge;
			HalfEdge* HE2 = HE1->nextEdge;
			HalfEdge* HE3 = HE2->nextEdge;
			HalfEdge* HE4 = HE3->nextEdge;
			HalfEdge* HE5 = HE4->nextEdge;
			HalfEdge* HE6 = HE5->nextEdge;
			HalfEdge* HE7 = HE6->nextEdge;

			HalfEdge* HE1next = new HalfEdge();
			HalfEdge* HE2next = new HalfEdge();
			HalfEdge* HE3next = new HalfEdge();
			HalfEdge* HE4next = new HalfEdge();

			HalfEdge* HE1pair = new HalfEdge();
			HalfEdge* HE2pair = new HalfEdge();
			HalfEdge* HE3pair = new HalfEdge();
			HalfEdge* HE4pair = new HalfEdge();

			Face* newFace1 = new Face();
			Face* newFace2 = new Face();
			Face* newFace3 = new Face();

			HE1next->start = HE2->start;
			HE1next->nextEdge = HE4pair;
			HE1next->pairEdge = HE1pair;
			HE1next->f = f;

			HE2next->start = HE4->start;
			HE2next->nextEdge = HE1pair;
			HE2next->pairEdge = HE2pair;
			HE2next->f = newFace1;

			HE3next->start = HE6->start;
			HE3next->nextEdge = HE2pair;
			HE3next->pairEdge = HE3pair;
			HE3next->f = newFace2;

			HE4next->start = current->start;
			HE4next->nextEdge = HE3pair;
			HE4next->pairEdge = HE4pair;
			HE4next->f = newFace3;

			//pairs
			HE1pair->start = f->center;
			HE1pair->nextEdge = HE2;
			HE1pair->pairEdge = HE1next;
			HE1pair->f = newFace1;

			HE2pair->start = f->center;
			HE2pair->nextEdge = HE4;
			HE2pair->pairEdge = HE2next;
			HE2pair->f = newFace2;

			HE3pair->start = f->center;
			HE3pair->nextEdge = HE6;
			HE3pair->pairEdge = HE3next;
			HE3pair->f = newFace3;

			HE4pair->start = f->center;
			HE4pair->nextEdge = current;
			HE4pair->pairEdge = HE4next;
			HE4pair->f = f;

			HE1->nextEdge = HE1next;
			HE3->nextEdge = HE2next;
			HE5->nextEdge = HE3next;
			HE7->nextEdge = HE4next;

			HE2->f = newFace1;
			HE3->f = newFace1;
			HE4->f = newFace2;
			HE5->f = newFace2;
			HE6->f = newFace3;
			HE7->f = newFace3;

			newFace1->e = HE2;
			newFace2->e = HE4;
			newFace3->e = HE6;

			newFace1->id = mesh->idCounter;
			mesh->idCounter++;
			newFace2->id = mesh->idCounter;
			mesh->idCounter++;
			newFace3->id = mesh->idCounter;
			mesh->idCounter++;

			pickedIDs->push_back(newFace1->id);
			pickedIDs->push_back(newFace2->id);
			pickedIDs->push_back(newFace3->id);

			newFaces.push_back(newFace1);
			newFaces.push_back(newFace2);
			newFaces.push_back(newFace3);

			f->e = current;
			f->center->e = HE1pair;
		}
		else
		{
			HalfEdge* HE1 = current->nextEdge;
			HalfEdge* HE2 = HE1->nextEdge;
			HalfEdge* HE3 = HE2->nextEdge;
			HalfEdge* HE4 = HE3->nextEdge;
			HalfEdge* HE5 = HE4->nextEdge;

			HalfEdge* HE1next = new HalfEdge();
			HalfEdge* HE2next = new HalfEdge();
			HalfEdge* HE3next = new HalfEdge();

			HalfEdge* HE1pair = new HalfEdge();
			HalfEdge* HE2pair = new HalfEdge();
			HalfEdge* HE3pair = new HalfEdge();

			Face* newFace1 = new Face();
			Face* newFace2 = new Face();

			HE1next->start = HE2->start;
			HE1next->nextEdge = HE3pair;
			HE1next->pairEdge = HE1pair;
			HE1next->f = f;

			HE2next->start = HE4->start;
			HE2next->nextEdge = HE1pair;
			HE2next->pairEdge = HE2pair;
			HE2next->f = newFace1;

			HE3next->start = current->start;
			HE3next->nextEdge = HE2pair;
			HE3next->pairEdge = HE3pair;
			HE3next->f = newFace2;

			//pairs
			HE1pair->start = f->center;
			HE1pair->nextEdge = HE2;
			HE1pair->pairEdge = HE1next;
			HE1pair->f = newFace1;

			HE2pair->start = f->center;
			HE2pair->nextEdge = HE4;
			HE2pair->pairEdge = HE2next;
			HE2pair->f = newFace2;

			HE3pair->start = f->center;
			HE3pair->nextEdge = current;
			HE3pair->pairEdge = HE3next;
			HE3pair->f = f;

			HE1->nextEdge = HE1next;
			HE3->nextEdge = HE2next;
			HE5->nextEdge = HE3next;

			HE2->f = newFace1;
			HE3->f = newFace1;
			HE4->f = newFace2;
			HE5->f = newFace2;

			newFace1->e = HE2;
			newFace2->e = HE4;

			newFace1->id = mesh->idCounter;
			mesh->idCounter++;
			newFace2->id = mesh->idCounter;
			mesh->idCounter++;

			pickedIDs->push_back(newFace1->id);
			pickedIDs->push_back(newFace2->id);

			newFaces.push_back(newFace1);
			newFaces.push_back(newFace2);

			f->e = current;
			f->center->e = HE1pair;
		}
	}

	std::cout << "FACES COMPLETE" << std::endl;

	for (Vertex* v0 : selectedFaceVerts)
	{
		float nADJ = 0.0f;
		glm::vec3 ve = glm::vec3(0.0f);
		glm::vec3 vf = glm::vec3(0.0f);
		HalfEdge* current = v0->e; // MIGHT NOT HAVE ASSIGNED THIS IN PROGRAM.CPP AFTER OBJLOADING
		do
		{
			ve+=current->pairEdge->start->v;
			vf+=current->nextEdge->nextEdge->start->v;
			current = current->pairEdge->nextEdge;
			nADJ++;
		} while (current != v0->e);
		v0->v = (nADJ-2)*v0->v/nADJ+ve/(nADJ*nADJ)+vf/(nADJ*nADJ);

	}

	for (Face* f:unselectedAdjFaces)
	{
		HalfEdge* current = f->e;
		int nVerts = 0;
		do
		{
			current = current->nextEdge;
			nVerts++;
		} while (current!= f->e);
		current = f->e;

		if (nVerts == 4)
		{
			while (find(EVs.begin(), EVs.end(), current->start) == EVs.end())
				current = current->nextEdge;

			HalfEdge* HE1 = current->nextEdge;
			HalfEdge* HE2 = HE1->nextEdge;
			HalfEdge* HE3 = HE2->nextEdge;

			Face* newF = new Face();

			HalfEdge* HEnext = new HalfEdge();
			HalfEdge* HEpair = new HalfEdge();

			HEnext->start = HE2->start;
			HEnext->nextEdge = current;
			HEnext->pairEdge = HEpair;
			HEnext->f = f;

			HEpair->start = current->start;
			HEpair->nextEdge = HE2;
			HEpair->pairEdge = HEnext;
			HEpair->f = newF;

			HE1->nextEdge = HEnext;
			HE3->nextEdge = HEpair;

			HE3->f = newF;
			HE2->f = newF;

			newF->e = HEpair;
			newF->id = mesh->idCounter;

			f->e = HEnext;

			mesh->idCounter++;
			newFaces.push_back(newF);
		}
		if (nVerts == 5)
		{
			while (find(EVs.begin(), EVs.end(), current->start) == EVs.end())
				current = current->nextEdge;

			HalfEdge* HE1next = new HalfEdge();
			HalfEdge* HE1pair = new HalfEdge();
			HalfEdge* HE2next = new HalfEdge();
			HalfEdge* HE2pair = new HalfEdge();
			HalfEdge* HE1 = current->nextEdge;
			HalfEdge* HE2 = HE1->nextEdge;
			HalfEdge* HE3 = HE2->nextEdge;
			HalfEdge* HE4 = HE3->nextEdge;
			Face* newF1 = new Face();
			Face* newF2 = new Face();

			HE1->nextEdge = HE1next;//HE1
			HE1next->nextEdge = current;
			HE1next->start = HE2->start;
			HE1pair->start = current->start;
			newF1->e = HE1next;
			newF1->id = mesh->idCounter;
			mesh->idCounter++;

			HE4->nextEdge = HE2next;
			HE2next->nextEdge = HE3;
			HE2next->start = current->start;
			HE2pair->start = HE3->start;
			newF2->e = HE2next;
			newF2->id = mesh->idCounter;
			mesh->idCounter++;

			HE1next->pairEdge = HE1pair;
			HE1pair->pairEdge = HE1next;
			HE2next->pairEdge = HE2pair;
			HE2pair->pairEdge = HE2next;

			current->f = newF1;
			HE1next->f = newF1;
			HE1pair->f = f;
			HE2next->f = newF2;
			HE2pair->f = f;
			HE1->f = newF1;
			HE3->f = newF2;
			HE4->f = newF2;

			HE2pair->nextEdge = HE1pair;
			HE1pair->nextEdge = HE2;
			HE2->nextEdge = HE2pair;//nextEdge

			f->e = HE2;

			newFaces.push_back(newF1);
			newFaces.push_back(newF2);
		}
		if (nVerts == 6)
		{
			while (find(EVs.begin(), EVs.end(), current->start) == EVs.end())
				current = current->nextEdge;

			if (find(EVs.begin(), EVs.end(), current->nextEdge->nextEdge->nextEdge->start) != EVs.end())
			{
				HalfEdge* HE1 = current->nextEdge;
				HalfEdge* HE2 = HE1->nextEdge;
				HalfEdge* HE3 = HE2->nextEdge;
				HalfEdge* HE4 = HE3->nextEdge;
				HalfEdge* HE5 = HE4->nextEdge;

				Face* newF = new Face();
				HalfEdge* HEnext = new HalfEdge();
				HalfEdge* HEpair = new HalfEdge();

				HEnext->start = HE3->start;
				HEnext->nextEdge = current;
				HEnext->pairEdge = HEpair;
				HEnext->f = f;

				HEpair->start = current->start;
				HEpair->nextEdge = HE3;
				HEpair->pairEdge = HEnext;
				HEpair->f = newF;

				HE2->nextEdge = HEnext;
				HE5->nextEdge = HEpair;

				HE5->f = newF;
				HE4->f = newF;
				HE3->f = newF;

				newF->e = HEpair;
				newF->id = mesh->idCounter;

				f->e = HEnext;

				mesh->idCounter++;
				newFaces.push_back(newF);
			}
			else
			{
				HalfEdge* HE1next = new HalfEdge();
				HalfEdge* HE1pair = new HalfEdge();
				HalfEdge* HE2next = new HalfEdge();
				HalfEdge* HE2pair = new HalfEdge();
				HalfEdge* HE3next = new HalfEdge();
				HalfEdge* HE3pair = new HalfEdge();
				HalfEdge* HE1 = current->nextEdge;
				HalfEdge* HE2 = HE1->nextEdge;
				HalfEdge* HE3 = HE2->nextEdge;
				HalfEdge* HE4 = HE3->nextEdge;
				HalfEdge* HE5 = HE4->nextEdge;
				Face* newF1 = new Face();
				Face* newF2 = new Face();
				Vertex* vc = new Vertex();
				vc = f->center;
				vc->e = HE1pair;

				HE1next->start = HE2->start;
				HE2next->start = HE4->start;
				HE3next->start = current->start;
				HE1pair->start = vc;
				HE2pair->start = vc;
				HE3pair->start = vc;

				HE1next->nextEdge = HE3pair;
				HE2next->nextEdge = HE1pair;
				HE3next->nextEdge = HE2pair;
				HE1pair->nextEdge = HE2;
				HE2pair->nextEdge = HE4;
				HE3pair->nextEdge = current;

				HE1->nextEdge = HE1next;
				HE3->nextEdge = HE2next;
				HE5->nextEdge = HE3next;

				HE1next->f = f;
				HE3pair->f = f;
				f->e = current;

				HE2next->f = newF1;
				HE1pair->f = newF1;
				HE2->f = newF1;
				HE3->f = newF1;
				newF1->e = HE2;

				HE3next->f = newF2;
				HE2pair->f = newF2;
				HE4->f = newF2;
				HE5->f = newF2;
				newF2->e = HE4;

				newF1->id = mesh->idCounter;
				mesh->idCounter++;
				newF2->id = mesh->idCounter;
				mesh->idCounter++;

				HE1next->pairEdge = HE1pair;
				HE1pair->pairEdge = HE1next;
				HE2next->pairEdge = HE2pair;
				HE2pair->pairEdge = HE2next;
				HE3next->pairEdge = HE3pair;
				HE3pair->pairEdge = HE3next;

				newFaces.push_back(newF1);
				newFaces.push_back(newF2);
				FVs.push_back(vc);

			}
		}
		if (nVerts == 7) {
			while (find(EVs.begin(), EVs.end(), current->start) == EVs.end())
				current = current->nextEdge;

			HalfEdge* HE1next = new HalfEdge();
			HalfEdge* HE1pair = new HalfEdge();
			HalfEdge* HE2next = new HalfEdge();
			HalfEdge* HE2pair = new HalfEdge();
			HalfEdge* HE3next = new HalfEdge();
			HalfEdge* HE3pair = new HalfEdge();
			HalfEdge* HE4next = new HalfEdge();
			HalfEdge* HE4pair = new HalfEdge();
			HalfEdge* HE5next = new HalfEdge();
			HalfEdge* HE5pair = new HalfEdge();
			HalfEdge* HE1 = current->nextEdge;
			HalfEdge* HE2 = HE1->nextEdge;
			HalfEdge* HE3 = HE2->nextEdge;
			HalfEdge* HE4 = HE3->nextEdge;
			HalfEdge* HE5 = HE4->nextEdge;
			HalfEdge* HE6 = HE5->nextEdge;
			Face* newF1 = new Face();
			Face* newF2 = new Face();
			Face* newF3 = new Face();
			Face* newF4 = new Face();
			Vertex* vc = new Vertex();
			vc = f->center;
			vc->e - HE1pair;

			HE1pair->start = vc;
			HE2pair->start = vc;
			HE3pair->start = vc;
			HE4pair->start = vc;
			HE5pair->start = vc;

			HE1next->pairEdge = HE1pair;
			HE1pair->pairEdge = HE1next;
			HE2next->pairEdge = HE2pair;
			HE2pair->pairEdge = HE2next;
			HE3next->pairEdge = HE3pair;
			HE3pair->pairEdge = HE3next;
			HE4next->pairEdge = HE4pair;
			HE4pair->pairEdge = HE4next;
			HE5next->pairEdge = HE5pair;
			HE5pair->pairEdge = HE5next;

			HE1next->nextEdge = HE5pair;
			HE2next->nextEdge = HE1pair;
			HE3next->nextEdge = HE2pair;
			HE4next->nextEdge = HE3pair;
			HE5next->nextEdge = HE4pair;

			if (find(EVs.begin(), EVs.end(), current->nextEdge->nextEdge->start) != EVs.end() ) { // current is a small square
				if (find(EVs.begin(), EVs.end(), current->nextEdge->nextEdge->nextEdge->nextEdge->start) != EVs.end() ) { //next is other small square
					HE1next->start = HE2->start;
					HE2next->start = HE4->start;
					HE3next->start = HE5->start;
					HE4next->start = HE6->start;
					HE5next->start = current->start;

					HE1->nextEdge = HE1next;
					HE3->nextEdge = HE2next;
					HE4->nextEdge = HE3next;
					HE5->nextEdge = HE4next;
					HE6->nextEdge = HE5next;

					HE1pair->nextEdge = HE2;
					HE2pair->nextEdge = HE4;
					HE3pair->nextEdge = HE5;
					HE4pair->nextEdge = HE6;
					HE5pair->nextEdge = current;

					HE1next->f = f;
					HE5pair->f = f;
					f->e = current;

					HE2->f = newF1;
					HE3->f = newF1;
					HE2next->f = newF1;
					HE1pair->f = newF1;
					newF1->e = HE2;

					HE4->f = newF2;
					HE3next->f = newF2;
					HE2pair->f = newF2;
					newF2->e = HE4;

					HE5->f = newF3;
					HE4next->f = newF3;
					HE3pair->f = newF3;
					newF3->e = HE5;

					HE6->f = newF4;
					HE5next->f = newF4;
					HE4pair->f = newF4;
					newF4->e = HE6;

				}
				else{ //next is big rectangle
					HE1next->start = HE2->start;
					HE2next->start = HE3->start;
					HE3next->start = HE4->start;
					HE4next->start = HE5->start;
					HE5next->start = current->start;

					HE1->nextEdge = HE1next;
					HE2->nextEdge = HE2next;
					HE3->nextEdge = HE3next;
					HE4->nextEdge = HE4next;
					HE6->nextEdge = HE5next;

					HE1pair->nextEdge = HE2;
					HE2pair->nextEdge = HE3;
					HE3pair->nextEdge = HE4;
					HE4pair->nextEdge = HE5;
					HE5pair->nextEdge = current;

					HE1next->f = f;
					HE5next->f = f;
					f->e = current;

					HE2->f = newF1;
					HE2next->f = newF1;
					HE1pair->f = newF1;
					newF1->e = HE2;

					HE3->f = newF2;
					HE3next->f = newF2;
					HE2pair->f = newF2;
					newF2->e = HE3;

					HE4->f = newF3;
					HE4next->f = newF3;
					HE3pair->f = newF3;
					newF3->e = HE4;

					HE5->f = newF4;
					HE6->f = newF4;
					HE5next->f = newF4;
					HE4pair->f = newF4;
					newF4->e = HE5;
				}
			}
			else { //current is big rectangle
				HE1next->start = HE1->start;
				HE2next->start = HE2->start;
				HE3next->start = HE3->start;
				HE4next->start = HE5->start;
				HE5next->start = current->start;

				HE1->nextEdge = HE2next;
				HE2->nextEdge = HE3next;
				HE4->nextEdge = HE4next;
				HE6->nextEdge = HE5next;
				current->nextEdge = HE1next;

				HE1pair->nextEdge = HE1;
				HE2pair->nextEdge = HE2;
				HE3pair->nextEdge = HE3;
				HE4pair->nextEdge = HE5;
				HE5pair->nextEdge = current;

				HE1next->f = f;
				HE5next->f = f;
				f->e = current;

				HE1->f = newF1;
				HE2next->f = newF1;
				HE1pair->f = newF1;
				newF1->e = HE1;

				HE2->f = newF2;
				HE3next->f = newF2;
				HE2pair->f = newF2;
				newF2->e = HE2;

				HE3->f = newF3;
				HE4->f = newF3;
				HE4next->f = newF3;
				HE3pair->f = newF3;
				newF3->e = HE3;

				HE5->f = newF4;
				HE6->f = newF4;
				HE5next->f = newF4;
				HE4pair->f = newF4;
				newF4->e = HE5;
			}
			newF1->id = mesh->idCounter;
			mesh->idCounter++;
			newF2->id = mesh->idCounter;
			mesh->idCounter++;
			newF3->id = mesh->idCounter;
			mesh->idCounter++;
			newF4->id = mesh->idCounter;
			mesh->idCounter++;

			newFaces.push_back(newF1);
			newFaces.push_back(newF2);
			newFaces.push_back(newF3);
			newFaces.push_back(newF4);
			FVs.push_back(vc);
		}
		if (nVerts == 8)
		{
			while (find(EVs.begin(), EVs.end(), current->start) == EVs.end())
				current = current->nextEdge;

			HalfEdge* HE1 = current->nextEdge;
			HalfEdge* HE2 = HE1->nextEdge;
			HalfEdge* HE3 = HE2->nextEdge;
			HalfEdge* HE4 = HE3->nextEdge;
			HalfEdge* HE5 = HE4->nextEdge;
			HalfEdge* HE6 = HE5->nextEdge;
			HalfEdge* HE7 = HE6->nextEdge;

			HalfEdge* HE1next = new HalfEdge();
			HalfEdge* HE2next = new HalfEdge();
			HalfEdge* HE3next = new HalfEdge();
			HalfEdge* HE4next = new HalfEdge();

			HalfEdge* HE1pair = new HalfEdge();
			HalfEdge* HE2pair = new HalfEdge();
			HalfEdge* HE3pair = new HalfEdge();
			HalfEdge* HE4pair = new HalfEdge();

			Face* newFace1 = new Face();
			Face* newFace2 = new Face();
			Face* newFace3 = new Face();

			HE1next->start = HE2->start;
			HE1next->nextEdge = HE4pair;
			HE1next->pairEdge = HE1pair;
			HE1next->f = f;

			HE2next->start = HE4->start;
			HE2next->nextEdge = HE1pair;
			HE2next->pairEdge = HE2pair;
			HE2next->f = newFace1;

			HE3next->start = HE6->start;
			HE3next->nextEdge = HE2pair;
			HE3next->pairEdge = HE3pair;
			HE3next->f = newFace2;

			HE4next->start = current->start;
			HE4next->nextEdge = HE3pair;
			HE4next->pairEdge = HE4pair;
			HE4next->f = newFace3;

			//pairs
			HE1pair->start = f->center;
			HE1pair->nextEdge = HE2;
			HE1pair->pairEdge = HE1next;
			HE1pair->f = newFace1;

			HE2pair->start = f->center;
			HE2pair->nextEdge = HE4;
			HE2pair->pairEdge = HE2next;
			HE2pair->f = newFace2;

			HE3pair->start = f->center;
			HE3pair->nextEdge = HE6;
			HE3pair->pairEdge = HE3next;
			HE3pair->f = newFace3;

			HE4pair->start = f->center;
			HE4pair->nextEdge = current;
			HE4pair->pairEdge = HE4next;
			HE4pair->f = f;

			HE1->nextEdge = HE1next;
			HE3->nextEdge = HE2next;
			HE5->nextEdge = HE3next;
			HE7->nextEdge = HE4next;

			HE2->f = newFace1;
			HE3->f = newFace1;
			HE4->f = newFace2;
			HE5->f = newFace2;
			HE6->f = newFace3;
			HE7->f = newFace3;

			newFace1->e = HE2;
			newFace2->e = HE4;
			newFace3->e = HE6;

			newFace1->id = mesh->idCounter;
			mesh->idCounter++;
			newFace2->id = mesh->idCounter;
			mesh->idCounter++;
			newFace3->id = mesh->idCounter;
			mesh->idCounter++;

			newFaces.push_back(newFace1);
			newFaces.push_back(newFace2);
			newFaces.push_back(newFace3);

			f->e = current;
			f->center->e = HE1pair;
		}
	}

	// add the new vertices to the lsit of vertices
	for (Vertex* v0 : FVs)
		mesh->vertices.push_back(v0);
	for (Vertex* v0 : EVs)
		mesh->vertices.push_back(v0);
	for (Face* f0 : newFaces) {
		mesh->faces.push_back(f0);
	}

	//Creates a lsit of new vertices per face
	for (Face* f : mesh->faces)
	{
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

		f->center = v0;
		f->center->vn = glm::normalize(glm::cross(
			f->e->nextEdge->start->v - f->e->start->v,
			f->e->nextEdge->nextEdge->start->v - f->e->start->v));
	}

	this->clearGeometry();
	this->makeModel(mesh->faces);

	std::cout << "SUBDIVISION COMPLETE" << std::endl;
}


void Geometry::stretchFace(Mesh* mesh, std::vector<int> *pickedIDs, float d) {
//	std::vector<Face*> newFaces;
	std::vector<Face*> selectedFaces;
//	std::vector<Face*> unselectedAdjFaces;
	
	std::vector<Vertex*> selectedFaceVerts;
	std::vector<glm::vec3> selectedVertsDirection;
//	std::vector<Vertex*> newVerts;

	// makes list of selected faces
	for (Face* f : mesh->faces)
	{
		if (find(pickedIDs->begin(), pickedIDs->end(), f->id) != pickedIDs->end())
			selectedFaces.push_back(f);
	}
	for (Face* f : selectedFaces) {
		HalfEdge* current = f->e;
		do {
			// finds selected face vertices (all should be pulled)
			if (find(selectedFaceVerts.begin(), selectedFaceVerts.end(), current->start) == selectedFaceVerts.end()) {
				selectedFaceVerts.push_back(current->start);
				glm::vec3 vd = glm::vec3(0.0f);
				float nFaces = 0.0f;
				HalfEdge* cur = current->start->e;
				do {
					// find average vn on current->start for direction of elevation
					vd += cur->f->center->vn;
					nFaces++;					
					cur = cur->pairEdge->nextEdge;
				} while (cur != current->start->e);	
				vd = vd/nFaces;
				selectedVertsDirection.push_back(vd);
//				selectedVertsDirection.push_back(f->center->vn);
			}
			current = current->nextEdge;
		} while (current!= f->e);
	}
	for (int i=0; i<selectedFaceVerts.size(); i++) {
		selectedFaceVerts[i]->v += d*selectedVertsDirection[i];
	}
	for (Face* f : mesh->faces)
	{
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

		f->center = v0;
		f->center->vn = glm::normalize(glm::cross(
			f->e->nextEdge->start->v - f->e->start->v,
			f->e->nextEdge->nextEdge->start->v - f->e->start->v));
	}
	this->clearGeometry();
	this->makeModel(mesh->faces);

}

void Geometry::pullFace(Mesh* mesh, std::vector<int> *pickedIDs, float d) {
	std::vector<Face*> newFaces;
	std::vector<HalfEdge*> border;
	std::vector<Face*> selectedFaces;
	std::vector<Face*> unselectedAdjFaces;
	
	std::vector<std::vector<HalfEdge*>> islands;
	
	std::vector<Vertex*> selectedFaceVerts;
	std::vector<glm::vec3> selectedVertsDirection;
	std::vector<Vertex*> newVerts;

	// makes list of selected faces
	for (Face* f : mesh->faces)
	{
		if (find(pickedIDs->begin(), pickedIDs->end(), f->id) != pickedIDs->end())
			selectedFaces.push_back(f);
	}
	for (Face* f : selectedFaces) {
		HalfEdge* current = f->e;
		do {
			// finds selected face vertices (all should be pulled)
			if (find(selectedFaceVerts.begin(), selectedFaceVerts.end(), current->start) == selectedFaceVerts.end()) {
				selectedFaceVerts.push_back(current->start);
//				glm::vec3 vd = glm::vec3(0.0f);
//				float nFaces = 0.0f;
				HalfEdge* cur = current->start->e;
				do {
					// adding border vertices
					if (find(selectedFaces.begin(), selectedFaces.end(), cur->f) == selectedFaces.end()) {
//						Vertex* v0 = new Vertex();
						
//						Face* newF = new Face()
//						HalfEdge* HE0 = new HalfEdge();
//						HE0 = current;
//						HalfEdge* HE1 = new HalfEdge();
//						HalfEdge* HE2 = new HalfEdge();
//						HalfEdge* HE3 = new HalfEdge();
//						HalfEdge* HE4 = new HalfEdge();
//						v0->v.x = cur->start->v.x;
//						v0->v.y = cur->start->v.y;
//						v0->v.z = cur->start->v.z;
//						v0 = cur->start;
//						newVerts.push_back(v0);
						border.push_back(current);
						break;
					}
					
					// find average vn on current->start for direction of elevation
//					vd += cur->f->center->vn;
//					nFaces++;					
					cur = cur->pairEdge->nextEdge;
				} while (cur != current->start->e);	
//				vd = vd/nFaces;
//				selectedVertsDirection.push_back(vd);
				selectedVertsDirection.push_back(f->center->vn);
			}
			current = current->nextEdge;
		} while (current!= f->e);
	}

	
	
//	nIslands = 0;
	while (border.size() >0) {
		std::vector<HalfEdge*> nisland;
		HalfEdge* startBorder = border[0];
		HalfEdge* currentBorder = startBorder;
		do {
			if (find(selectedFaces.begin(), selectedFaces.end(), currentBorder->pairEdge->f) == selectedFaces.end()) {
				nisland.push_back(currentBorder->pairEdge);
				border.erase(border.begin());
//				border.erase(find(border.begin(),border.end(),currentBorder));
				currentBorder = currentBorder->nextEdge;
			}
			else {
				
				currentBorder = currentBorder->pairEdge->nextEdge;
			}
		} while(currentBorder != startBorder);
		islands.push_back(nisland);
	}
	for (std::vector<HalfEdge*> island : islands) {
		for (int i=0; i<island.size(); i++) {
			HalfEdge* current = island[i];
//			HalfEdge* pair = current->pairEdge;
			Face* newf = new Face();
			Vertex* v0 = new Vertex();
			v0->v = current->start->v;
			HalfEdge* HE1 = new HalfEdge();
			HalfEdge* HE2 = new HalfEdge();
			HalfEdge* HE3 = new HalfEdge();
			HalfEdge* HE4 = new HalfEdge();
			
			
			HE1->start = current->pairEdge->start;
			HE2->start = current->nextEdge->start;
			HE3->start = v0;
			HE4->start = v0;
			

			HE1->nextEdge = HE2;
			HE2->nextEdge = HE4;
			HE4->nextEdge = current;
			HE3->nextEdge = current->nextEdge;
			current->nextEdge->nextEdge->nextEdge->nextEdge = HE3;
			current->nextEdge = HE1;
			
			HE1->f = newf;
			HE2->f = newf;
			HE4->f = newf;
			HE3->f = current->f;
			current->f = newf;

			
			newf->id = mesh->idCounter;
			mesh->idCounter++;
			HE2->pairEdge = HE3;
			HE3->pairEdge = HE2;
			
			v0->e = HE4;
			newf->e = HE4; 
			current->f->e = HE3;
			

			if (i>0) {
				HE1->pairEdge = island[i-1]->f->e;
				island[i-1]->f->e->pairEdge = HE1;
//				current->nextEdge->start = island[i-1]->f->e->start;
/*				HalfEdge* cur = island[i-1]->f->e;
				Vertex* cv = cur->start;
				do {
					cur->start = cv;
					std::cout<< "aaaa" << std::endl;
					cur = cur->pairEdge->nextEdge;
				}while (cur != island[i-1]->f->e);*/

			}
			if (i==island.size()-1) {
				HE4->pairEdge = island[0]->nextEdge;
				island[0]->nextEdge->pairEdge = HE4;
/*				HalfEdge* cur = HE4;
				Vertex* cv = cur->start;
				do {
					cur->start = cv;
					std::cout<< "dddd" << std::endl;
					cur = cur->pairEdge->nextEdge;
				}while (cur != HE4);*/
//				HE4->pairEdge = island[0]->f->e->nextEdge->nextEdge;
//				island[0]->f->e->nextEdge->nextEdge->pairEdge = HE4;
			}
			newVerts.push_back(v0);
			newFaces.push_back(newf);
			
		}
	}
/*	for (std::vector<HalfEdge*> island : islands) {
		for (int i=0; i<island.size(); i++) {
			HalfEdge* current = island[i];
			HalfEdge* cur = island[i]->f->e;
			Vertex* cv = cur->start;
			do {
				cur->start = cv;
				cur = cur->pairEdge->nextEdge;
			}while (cur != island[i]->f->e);
			
		}
	}
*/
	
//	std::cout << newVerts.size() << std::endl;
//	std::cout << newFaces.size() << std::endl;
	for (Vertex* v0 : newVerts)
		mesh->vertices.push_back(v0);
	for (Face* f0 : newFaces) {
		mesh->faces.push_back(f0);
	}
	for (int i=0; i<selectedFaceVerts.size(); i++) {
		selectedFaceVerts[i]->v += d*selectedVertsDirection[i];
	}
	for (Face* f : mesh->faces)
	{
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

		f->center = v0;
		f->center->vn = glm::normalize(glm::cross(
			f->e->nextEdge->start->v - f->e->start->v,
			f->e->nextEdge->nextEdge->start->v - f->e->start->v));
	}
	this->clearGeometry();
	this->makeModel(mesh->faces);

}


void Geometry::writeToOBJ(Mesh * mesh, std::string path) {
	std::string objName;
	std::cout << "Enter object's name: ";
	std::cin >> objName;
//	std::vector<glm::vec3> faceVertices;
	std::vector<glm::vec3> faceNormals;
	std::map<Vertex*,int> vIndices;
//	for (Vertex* v0 : mesh->vertices) {
	for (int i=0; i<mesh->vertices.size(); i++) {
		vIndices[mesh->vertices[i]]=i;
	}
	for (Face* f : mesh->faces) {
		HalfEdge* current = f->e;
/*		faceNormals.push_back(glm::normalize(glm::cross(
			current->nextEdge->start->v - current->start->v,
			current->nextEdge->nextEdge->start->v - current->start->v)));*/
		faceNormals.push_back(f->center->vn);
	}
	std::ofstream myfile(path);
	myfile << std::fixed;
	myfile << std::setprecision(6);
	if (myfile.is_open()) {
		myfile << "o " << objName << "\n";
		for (Vertex* v0 : mesh->vertices)
			myfile << "v " << v0->v.x <<" " << v0->v.y <<" " << v0->v.z << "\n";
		for (glm::vec3 vn : faceNormals) 
			myfile << "vn " << vn.x << " " << vn.y << " " << vn.z << "\n";
//		for (Face* f : mesh->faces) {
		for (int i=0; i< mesh->faces.size(); i++) {
			Face* f = mesh->faces[i];
			HalfEdge* current = f->e->nextEdge;
			myfile << "f";
			do {
				 myfile << " " << vIndices[current->start]+1 << "/0/" << i+1;
				current = current->nextEdge;
			} while (current!= f->e->nextEdge);
			myfile << "\n";
		}
	}
	myfile.close();
}

void Geometry::readFromOBJ(Mesh* mesh, char* path) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vnormals;
	std::vector<std::vector<int>> faces;
	std::vector<std::vector<int>> fnormals;
	bool res = loadOBJ(path, vertices, vnormals, faces, fnormals);
	
	std::vector<Vertex*> vList;
	std::vector<Face*> fList;
	std::map<std::pair<int, int>, HalfEdge*> Edges;

	for (glm::vec3 v : vertices)
	{
		Vertex* p = new Vertex();
		p->v = v;
		vList.push_back(p);
	}

	for (std::vector<int> f : faces)
	{
		Face* fa = new Face();
		fList.push_back(fa);
		fa->elevation = 0;
	}

	int faceIndex = 0;
	for (std::vector<int> face : faces)
	{
		for (int i=0; i<(int)face.size(); i++)
		{
			int u, v;
			u = i;
			if (u == (int)face.size()-1)
				v = 0;
			else
				v = u+1;
			std::pair<int,int> uv = std::make_pair(face[u],face[v]);

			Edges[uv] = new HalfEdge();
			Edges[uv]->sharpness = 0.0f;
			fList[faceIndex]->id = -1;

			Edges[uv]->f = fList[faceIndex];
			Edges[uv]->start = vList[face[u]-1];
			vList[face[u]-1]->e = Edges[uv];
			fList[faceIndex]->e = Edges[uv];
		}


		Vertex* v0 = new Vertex();
		v0->v = glm::vec3(0.0f);
		for (int i=0; i<(int)face.size(); i++)
		{
			int u, v, unext, vnext;
			u = i;
			if (u == (int)face.size()-1)
				v = 0;
			else
				v = u+1;

			unext = u + 1;
			vnext = v + 1;

			if (u >= (int)face.size() - 1) unext = 0;
			if (v >= (int)face.size() - 1) vnext = 0;

			std::pair<int,int> uv = std::make_pair(face[u],face[v]);
			std::pair<int,int> vu = std::make_pair(uv.second,uv.first);
			std::pair<int,int> uvnext = std::make_pair(face[unext],face[vnext]);

			Edges[uv]->nextEdge = Edges[uvnext];
			if (Edges.find(vu) != Edges.end())
			{
				Edges[uv]->pairEdge = Edges[vu];
				Edges[vu]->pairEdge = Edges[uv];
			}

			v0->v += Edges[uv]->start->v;
		}
		v0->v = v0->v / (float)face.size();
		fList[faceIndex]->center = v0;
		fList[faceIndex]->center->vn = glm::normalize(glm::cross(
			fList[faceIndex]->e->nextEdge->start->v - fList[faceIndex]->e->start->v,
			fList[faceIndex]->e->nextEdge->nextEdge->start->v - fList[faceIndex]->e->start->v));

		fList[faceIndex]->id = faceIndex;
		Geometry::EdgeIDs[faceIndex] = fList[faceIndex];
		faceIndex++;
	}

//	InputHandler::mesh = new Mesh();
	this->clearGeometry();
	this->modelMatrix = glm::mat4(1.f);
	mesh->faces.clear();
	mesh->vertices.clear();
	for (Face * f : fList)
		mesh->faces.push_back(f);
	for (Vertex * v : vList)
		mesh->vertices.push_back(v);
	mesh->idCounter = faceIndex;
	this->makeModel(mesh->faces);
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(90.0f), glm::vec3(-1.0f,0.0f,0.0f));
	
}
