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

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

int InputHandler::lastID = 0;
int InputHandler::pickedID = -1;
std::vector<int> InputHandler::pickedIDs;
bool InputHandler::multiPick = false;

std::map<int, HalfEdge*> Geometry::EdgeIDs;

void Program::mainLoop() {
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vnormals;
	std::vector<std::vector<int>> faces;
	std::vector<std::vector<int>> fnormals;
//	std::vector<std::vector<std::pair<int,int>>> faces;
	bool res = loadOBJ("src/male_head.obj", vertices, vnormals, faces, fnormals);
//	bool res = loadOBJ("src/uh60.obj", vertices, vnormals, faces, fnormals);

	std::vector<Vertex*> vList;
	std::vector<Face*> fList;
	std::map<std::pair<int, int>, HalfEdge*> Edges;
//	std::map<int, HalfEdge*> EdgeIDs;
	
	for (glm::vec3 v : vertices) {
		Vertex* p = new Vertex();
		p->v = v;
		vList.push_back(p);
	}
	for (std::vector<int> f : faces) {
//	for (int k=1000; k<1006;k++){
		Face* fa = new Face();
		fList.push_back(fa);
	}
//	Face* fa = new Face();
//		fList.push_back(fa);
	int faceIndex = 0;
	int idCounter = 15;
	for (std::vector<int> face : faces) {
//	for (int k=1000; k<1006; k++) {
//		std::vector<int> face = faces[k];
//	std::vector<int> face = faces[1000];
		
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
//			Edges[uv]->start->v = Edges[uv]->start->v;
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
				Geometry::EdgeIDs[idCounter] = Edges[uv];
				idCounter++;
//				idCounter = idCounter << 1;
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
			InputHandler::lastID = idCounter;
			Geometry* mesh = new Geometry();
			mesh->makeMesh(fList);
			mesh->modelMatrix = glm::rotate(mesh->modelMatrix, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
			mesh->modelMatrix = glm::rotate(mesh->modelMatrix, glm::radians(90.0f), glm::vec3(-1.0f,0.0f,0.0f));
				
			renderEngine->assignBuffers(*mesh);
			renderEngine->updateBuffers(*mesh);
			InputHandler::stuff.push_back(mesh);
			renderEngine->render(InputHandler::stuff,glm::mat4(1.f),0);

	InputHandler::setUp(renderEngine);	
//std::cout << "DFASDFDSA" << std::endl;
/*	for (Geometry* g : InputHandler::stuff) 		
		renderEngine->updateBuffers(*g);
	renderEngine->render(InputHandler::stuff, glm::mat4(1.f), 1);*/
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
