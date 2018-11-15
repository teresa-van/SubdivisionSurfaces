#include "Program.h"

Program::Program() {
	window = nullptr;
	renderEngine = nullptr;
}

// Error callback for glfw errors
void Program::error(int error, const char* description) {
	std::cerr << description << std::endl;
}

// Called to start the program
void Program::start() {
	setupWindow();
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK) {
		std::cerr << glewGetErrorString(err) << std::endl;
	}

	renderEngine = new RenderEngine(window);
	InputHandler::setUp(renderEngine);
	mainLoop();
}

// Creates GLFW window for the program and sets callbacks for input
void Program::setupWindow() {
	glfwSetErrorCallback(Program::error);
	if (glfwInit() == 0) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(1024, 1024, "Subdivision Surfaces", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // V-sync on

	glfwSetKeyCallback(window, InputHandler::key);
	glfwSetMouseButtonCallback(window, InputHandler::mouse);
	glfwSetCursorPosCallback(window, InputHandler::motion);
	glfwSetScrollCallback(window, InputHandler::scroll);
	glfwSetWindowSizeCallback(window, InputHandler::reshape);
}

bool InputHandler::lefthold = false;
glm::vec3 InputHandler::camPos = glm::vec3(0.0f,.0f,0.0f);
glm::vec3 InputHandler::camDir = glm::vec3(0.0f,0.0f, -1.0f);
glm::vec3 InputHandler::camUp = glm::vec3(0.0f, 1.0f, 0.0f);
float InputHandler::thetaX = 0.0f;
float InputHandler::thetaY = 0.0f;
double InputHandler::oldxclick = 0.0;
double InputHandler::oldyclick = 0.0;
double InputHandler::xclick = 0.0;
double InputHandler::yclick = 0.0;

bool InputHandler::moveLeft = false;
bool InputHandler::moveRight = false;
bool InputHandler::moveUp = false;
bool InputHandler::moveDown = false;
bool InputHandler::moveForward = false;
bool InputHandler::moveBack = false;

glm::vec3 InputHandler::camRight = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 InputHandler::camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 InputHandler::camSpherePos = glm::vec3(M_PI/2, M_PI/2, 50.0f);
glm::vec2 InputHandler::mousePos = glm::vec2(0, 0);


std::vector<Geometry*> InputHandler::stuff;

void Program::mainLoop() {
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vnormals;
	std::vector<std::vector<int>> faces;
	std::vector<std::vector<int>> fnormals;
//	std::vector<std::vector<std::pair<int,int>>> faces;
	bool res = loadOBJ("src/male_head.obj", vertices, vnormals, faces, fnormals);

	std::vector<Vertex*> vList;
	std::vector<Face*> fList;
	std::map<std::pair<int, int>, HalfEdge*> Edges;
	
	for (glm::vec3 v : vertices) {
		Vertex* p = new Vertex();
		p->v = v;
		vList.push_back(p);
	}
	for (std::vector<int> f : faces) {
		Face* fa = new Face();
		fList.push_back(fa);
	}
	
	int faceIndex = 0;
	int idCounter = 0;
	
	for (std::vector<int> face : faces) {
		
//		Face * f = new Face();
		for (int i=0; i<face.size(); i++) {
			int u, v;
			u = i;
			if (u == face.size()-1)
				v = 0;
			else
				v = u+1;
			std::pair<int,int> uv = std::make_pair(face[u],face[v]);
			Edges[uv] = new HalfEdge();
			Edges[uv]->sharpness = 0.0f;
			Edges[uv]->id = -1;
						
//			Edges[uv]->f = f;
			Edges[uv]->f = fList[faceIndex];
//			Vertex *v_u = new Vertex();
//			v_u->v = vertices[face[u]-1];
//			Vertex *v_v = new Vertex();
//			std::cout << v_u->v.x << " , " << v_u->v.y << " , " << v_u->v.z << std::endl;
//			std::cout << v_u->v.x << " , " << v_u->v.y << " , " << v_u->v.z << std::endl;
//			Edges[uv]->start = v_u;
			Edges[uv]->start = vList[face[u]-1];
//			v_u->e = Edges[uv];
//			f->e = Edges[uv];
			fList[faceIndex]->e = Edges[uv];
		}
		for (int i=0; i<face.size(); i++) {
			int u, v, unext, vnext;
			u = i;
			if (u == face.size()-1)
				v = 0;
			else
				v = u+1;
				
			unext = u + 1;
			vnext = v + 1;
			
			if (u >= face.size() - 1) unext = 0;
			if (v >= face.size() - 1) vnext = 0;
			
			std::pair<int,int> uv = std::make_pair(face[u],face[v]);
			std::pair<int,int> vu = std::make_pair(uv.second,uv.first);
			std::pair<int,int> uvnext = std::make_pair(face[unext],face[vnext]);
			

			
//			std::cout << u << "," << v << "\t" << unext << "," << vnext << std::endl;
			Edges[uv]->nextEdge = Edges[uvnext];
//			std::cout<< " : " << Edges.end() << std::endl;
			if (Edges.find(vu) != Edges.end()) {
				Edges[uv]->pairEdge = Edges[vu];
				Edges[vu]->pairEdge = Edges[uv];
				Edges[uv]->id = Edges[vu]->id;
				
//				std::cout << Edges[vu]->id << std::endl;
			}
			if (Edges[uv]->id == -1) {
				Edges[uv]->id = idCounter;
				idCounter++;
			}
//			std::cout <<"dfasdfsd" << std::endl;
//			else
//				std::cout << Edges[uv]->id << std::endl;
/*			Geometry *he = new Geometry();
			he->makeEdge(Edges[uv]->start->v, Edges[uv]->nextEdge->start->v);
			he->modelMatrix = glm::rotate(he->modelMatrix, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
			he->modelMatrix = glm::rotate(he->modelMatrix, glm::radians(90.0f), glm::vec3(-1.0f,0.0f,0.0f));
				
			renderEngine->assignBuffers(*he);
			InputHandler::stuff.push_back(he);*/
			
		}
		faceIndex++;
	}
		for (Face* f : fList) {
			std::vector<glm::vec3> facePoints;
			HalfEdge* curEdge = f->e;
			do {
				facePoints.push_back(curEdge->start->v);
				curEdge = curEdge->nextEdge;
			}
			while (curEdge != f->e);
			Geometry* fa = new Geometry();
			fa->makeFace(facePoints);
			fa->modelMatrix = glm::rotate(fa->modelMatrix, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
			fa->modelMatrix = glm::rotate(fa->modelMatrix, glm::radians(90.0f), glm::vec3(-1.0f,0.0f,0.0f));
				
			renderEngine->assignBuffers(*fa);
			InputHandler::stuff.push_back(fa);
		}
//		std::cout << fList.size() << std::endl;
	
	
	
	
	
	
//	for(int i=0; i<vertices0.size(); i++) {
//		vec3 v[vertices0[i].size()];
//		vec3 col[vertices0[i].size()];
//		vec2 uv[uvs[i].size()];
//		vec3 ns[normals[i].size()];
		
//		for(int j=0; j<vertices0[i].size(); j+=4) {
/*			bool isTriangle = false;
			if (vertices0[i][j+2] == vertices0[i][j+3]) 
				isTriangle = true;
				
			Face* face = new Face();
			Vertex* v0;// = vertices0[i][j];
			Vertex* v1;// = vertices0[i][j+1];
			Vertex* v2;// = vertices0[i][j+2];
			Vertex* v3;// = vertices0[i][j+3];
			v0->x = vertices0[i][j].x;
			v0->y = vertices0[i][j].y;
			v0->z = vertices0[i][j].z;
			v1->x = vertices0[i][j+1].x;
			v1->y = vertices0[i][j+1].y;
			v1->z = vertices0[i][j+1].z;
			v2->x = vertices0[i][j+2].x;
			v2->y = vertices0[i][j+2].y;
			v2->z = vertices0[i][j+2].z;
			v3->x = vertices0[i][j+3].x;
			v3->y = vertices0[i][j+3].y;
			v3->z = vertices0[i][j+3].z;
			
			
			Edges[std::make_pair(v0,v1)] = new HalfEdge();
			Edges[std::make_pair(v0,v1)]->f = face;
			Edges[std::make_pair(v1,v2)] = new HalfEdge();
			Edges[std::make_pair(v1,v2)]->f = face;
			if (isTriangle) {
				Edges[std::make_pair(v2,v0)] = new HalfEdge();
				Edges[std::make_pair(v2,v0)]->f = face;
			}
			else {
				Edges[std::make_pair(v2,v3)] = new HalfEdge();
				Edges[std::make_pair(v2,v3)]->f = face;
				Edges[std::make_pair(v3,v0)] = new HalfEdge();
				Edges[std::make_pair(v3,v0)]->f = face;
			}
			
			v0->e = Edges[std::make_pair(v0,v1)];
			v1->e = Edges[std::make_pair(v1,v2)];
			if (isTriangle)
				v2->e = Edges::make_pair(v2,v0)];
			else {
				v2->e = Edges[std::make_pair(v2,v3)];
				v3->e = Edges[std::make_pair(v3,v0)];
			}

			Edges[std::make_pair(v0,v1)]->nextEdge = Edges[std::make_pair(v1,v2)];
			if (isTriangle) {
				Edges[std::make_pair(v1,v2)]->nextEdge = Edges[std::make_pair(v2,v0)];
				Edges[std::make_pair(v2,v0)]->nextEdge = Edges[std::make_pair(v0,v1)];
			}
			else {
				Edges[std::make_pair(v1,v2)]->nextEdge = Edges[std::make_pair(v2,v3)];
				Edges[std::make_pair(v2,v3)]->nextEdge = Edges[std::make_pair(v3,v0)];
				Edges[std::make_pair(v3,v0)]->nextEdge = Edges[std::make_pair(v0,v1)];
			}
			
			if ( Edges.find( std::make_pair(v1,v0) ) != Edges.end() ) {
				Edges[std::make_pair(v0,v1)]->pairEdge = Edges[std::make_pair(v1,v0)];
				Edges[std::make_pair(v1,v0)]->pairEdge = Edges[std::make_pair(v0,v1)];
			}
			if ( Edges.find( std::make_pair(v2,v1) ) != Edges.end() ) {
				Edges[std::make_pair(v1,v2)]->pairEdge = Edges[std::make_pair(v2,v1)];
				Edges[std::make_pair(v2,v1)]->pairEdge = Edges[std::make_pair(v1,v2)];
			}
			if (isTriangle) {
				if ( Edges.find( std::make_pair(v2,v0) ) != Edges.end() ) {
					Edges[std::make_pair(v2,v0)]->pairEdge = Edges[std::make_pair(v0,v2)];
					Edges[std::make_pair(v0,v2)]->pairEdge = Edges[std::make_pair(v2,v0)];
				}
			}
			else {
				if ( Edges.find( std::make_pair(v2,v3) ) != Edges.end() ) {
					Edges[std::make_pair(v2,v3)]->pairEdge = Edges[std::make_pair(v3,v2)];
					Edges[std::make_pair(v3,v2)]->pairEdge = Edges[std::make_pair(v2,v3)];
				}
				if ( Edges.find( std::make_pair(v3,v0) ) != Edges.end() ) {
					Edges[std::make_pair(v3,v0)]->pairEdge = Edges[std::make_pair(v0,v3)];
					Edges[std::make_pair(v0,v3)]->pairEdge = Edges[std::make_pair(v3,v0)];
				}
			}*/
			
			
/*			Geometry* face = new Geometry();
			face->makeFace(vertices0[i][j], vertices0[i][j+1], vertices0[i][j+2], vertices0[i][j+3]);
			face->modelMatrix = glm::rotate(face->modelMatrix, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
			face->modelMatrix = glm::rotate(face->modelMatrix, glm::radians(90.0f), glm::vec3(-1.0f,0.0f,0.0f));
			renderEngine->assignBuffers(*face);
			InputHandler::stuff.push_back(face);*/
//			objects.push_back(face);
//			v[j] = 0.5f*vertices0[i][j]+vec3(0.0f,6.378f,0.0f);;
//			col[j]=vec3(1.0f, 0.0f, 0.0f);
//		}
//		std::cout<<InputHandler::stuff.size()<<std::endl;
//		for (int j=0; j<uvs[i].size(); j++)
//			uv[j]=uvs[i][j];
//		for (int j=0; j<normals[i].size(); j++) {
//			ns[j]=normals[i][j];
//		}
/*		if (!InitializeVAO(&shima[i]))
			cout << "Program failed to intialize geometry!" << endl;
		if(!LoadGeometry(&shima[i], v, uv, ns, vertices0[i].size()))
			cout << "Failed to load geometry" << endl;
		if (!InitializeTexture(&kaze[i], pics[tID[i]], GL_TEXTURE_2D))
			cout << "Program failed to initialize texture" << endl;
//		cout << vertices0[i].size() << endl;
*/
//	}
	InputHandler::setUp(renderEngine);	

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
//		renderEngine->render(objects, glm::mat4(1.f), 1);
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	delete renderEngine;
	glfwTerminate();
}
