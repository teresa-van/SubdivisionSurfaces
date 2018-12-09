#include "Program.h"

Program::Program()
{
	window = nullptr;
	renderEngine = nullptr;
}

// Error callback for glfw errors
void Program::error(int error, const char* description)
{
	std::cerr << description << std::endl;
}

// Called to start the program
void Program::start()
{
	setupWindow();
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK)
		std::cerr << glewGetErrorString(err) << std::endl;

	renderEngine = new RenderEngine(window);
	InputHandler::setUp(renderEngine);
	mainLoop();
}

// Creates GLFW window for the program and sets callbacks for input
void Program::setupWindow()
{
	glfwSetErrorCallback(Program::error);
	if (glfwInit() == 0)
		exit(EXIT_FAILURE);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);
//	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

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
bool InputHandler::righthold = false;
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
Mesh * InputHandler::mesh;

int InputHandler::lastID = 0;
int InputHandler::pickedID = -1;
std::vector<int> InputHandler::pickedIDs;
bool InputHandler::multiPick = false;

std::map<int, Face*> Geometry::EdgeIDs;

float InputHandler::elevate = 0.0f;

void Program::mainLoop()
{


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vnormals;
	std::vector<std::vector<int>> faces;
	std::vector<std::vector<int>> fnormals;

//	bool res = loadOBJ("cube.obj", vertices, vnormals, faces, fnormals);
	// bool res = loadOBJ("pyramid.obj", vertices, vnormals, faces, fnormals);
	 bool res = loadOBJ("head.obj", vertices, vnormals, faces, fnormals);
//	bool res = loadOBJ("uh60.obj", vertices, vnormals, faces, fnormals);

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

	InputHandler::mesh = new Mesh();
	for (Face * f : fList)
		InputHandler::mesh->faces.push_back(f);
	for (Vertex * v : vList)
		InputHandler::mesh->vertices.push_back(v);
	InputHandler::mesh->idCounter = faceIndex;

	InputHandler::lastID = faceIndex;
	Geometry* model = new Geometry();
	model->makeModel(fList);
	model->modelMatrix = glm::rotate(model->modelMatrix, glm::radians(180.0f), glm::vec3(0.0f,1.0f,0.0f));
	model->modelMatrix = glm::rotate(model->modelMatrix, glm::radians(90.0f), glm::vec3(-1.0f,0.0f,0.0f));
	renderEngine->assignBuffers(*model);
	renderEngine->updateBuffers(*model);
	InputHandler::stuff.push_back(model);

	renderEngine->render(InputHandler::stuff,glm::mat4(1.f),0);

	InputHandler::setUp(renderEngine);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);

	for (Geometry* g: InputHandler::stuff)
		renderEngine->deleteBuffers(*g);

	delete renderEngine;
	glfwTerminate();
}
