#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "RenderEngine.h"
#include "Geometry.h"

class InputHandler {

public:
	static void setUp(RenderEngine* renderEngine);

	static void key(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse(GLFWwindow* window, int button, int action, int mods);
	static void motion(GLFWwindow* window, double x, double y);
	static void scroll(GLFWwindow* window, double x, double y);
	static void reshape(GLFWwindow* window, int width, int height);

	static double xclick;
	static double yclick;

	static bool lefthold;
	static bool righthold;
	static glm::vec3 camPos;
	static glm::vec3 camDir;
	static glm::vec3 camUp;
	static float thetaX;
	static float thetaY;
	static double oldxclick;
	static double oldyclick;

	static bool moveLeft;
	static bool moveRight;
	static bool moveForward;
	static bool moveBack;
	static bool moveUp;
	static bool moveDown;

	static glm::vec3 camSpherePos;
	static glm::vec3 camRight;
	static glm::vec3 camTarget;
	static glm::vec2 mousePos;

	static void controller();
	static void cameraController(glm::vec3 dPos);

//	static std::vector<Geometry*

	static void renderGeometries();
	static std::vector<Geometry*> stuff;

	static int lastID;
//	static int idCounter;
	static int pickedID;
	static std::vector<int> pickedIDs;
	static bool multiPick;

	static float elevate;
	static Mesh * mesh;

private:
	static RenderEngine* renderEngine;

	static int mouseOldX;
	static int mouseOldY;

};
