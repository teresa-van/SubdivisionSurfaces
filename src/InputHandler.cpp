#include "InputHandler.h"

RenderEngine* InputHandler::renderEngine;
int InputHandler::mouseOldX;
int InputHandler::mouseOldY;

// Must be called before processing any GLFW events
void InputHandler::setUp(RenderEngine* renderEngine) {
	InputHandler::renderEngine = renderEngine;
//	Geometry cube;
//	cube.makeCube(glm::vec3(0.0f), .5f, .5f, .5f);
//	renderEngine->assignBuffers(cube);
//	renderEngine->updateBuffers(cube);
//	stuff.push_back(&cube);
//	rotAxis.makeLine(r0, r1);
	cameraController(glm::vec3(0.0f, 0.0f, 0.0f));
	renderGeometries();
	
}

// Callback for key presses
void InputHandler::key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(0);
	}
	if (key == GLFW_KEY_LEFT_SHIFT) {
		if (action == GLFW_PRESS||action == GLFW_REPEAT) 
			multiPick = true;
		else
			multiPick = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) 
		moveLeft = true;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) 
		moveBack = true;
	if (key == GLFW_KEY_D && action == GLFW_PRESS) 
		moveRight = true;
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
		moveForward = true;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) 
		moveUp = true;
	if (key == GLFW_KEY_F && action == GLFW_PRESS) 
		moveDown = true;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) 
		moveLeft = false;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) 
		moveBack = false;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) 
		moveRight = false;
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) 
		moveForward = false;
	if (key == GLFW_KEY_R && action == GLFW_RELEASE) 
		moveUp = false;
	if (key == GLFW_KEY_F && action == GLFW_RELEASE) 
		moveDown = false;
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS||action == GLFW_REPEAT)) 
		elevate = -1.0f;
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS||action == GLFW_REPEAT)) 
		elevate = 1.0f;
	if ((key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) && action == GLFW_RELEASE) 
		elevate = 0.0f;
	
/*	if (key == GLFW_KEY_A && (action == GLFW_PRESS||action == GLFW_REPEAT)) {
//			dx = dx+0.1f;
		camPos -= glm::vec3(sin(thetaX-M_PI/2.0f),0,cos(thetaX-M_PI/2.0f))*0.1f;
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS||action == GLFW_REPEAT)) {
//			dz = dz-0.1f;
		camPos -= glm::vec3(	cos(-thetaY) * sin(thetaX), sin(-thetaY),cos(-thetaY) * cos(thetaX))*0.1f;
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS||action == GLFW_REPEAT)) {
//			dx = dx-0.1f;
		camPos += glm::vec3(sin(thetaX-M_PI/2.0f),0,cos(thetaX-M_PI/2.0f))*0.1f;
	}
	if (key == GLFW_KEY_W && (action == GLFW_PRESS||action == GLFW_REPEAT)) {
//			dz = dz+0.1f;
		camPos += glm::vec3(	cos(-thetaY) * sin(thetaX), sin(-thetaY),cos(-thetaY) * cos(thetaX))*0.1f;
	}
	if (key == GLFW_KEY_R && (action == GLFW_PRESS||action == GLFW_REPEAT)) {
//			dy = dy+0.1f;
		camPos += glm::vec3(0.0f,0.1f,0.0f);
		
	}
	if (key == GLFW_KEY_F && (action == GLFW_PRESS||action == GLFW_REPEAT)) {
//			dy = dy-0.1f;
		camPos -= glm::vec3(0.0f,0.1f,0.0f);
		
	}*/
	renderGeometries();

}

// Callback for mouse button presses
void InputHandler::mouse(GLFWwindow* window, int button, int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	xclick = x;//(-512+x)/512;
	yclick = y;//(512-y)/512;
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{	
	//		std::cout << xclick<< "," << yclick << std::endl;
		if (!multiPick) 
			pickedIDs.clear();
		glFlush();
		glFinish();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		unsigned char data[4];
	//		int data[4];
		
		glReadPixels(x, 1024-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
		pickedID = ((data[0] + data[1]*256 + data[2]*256*256)) &0x00ffffff;
		if (pickedID == 0x00ffffff) {
	//			std::cout << pickedID << ": background" << std::endl;
			lefthold = true;
			pickedID = -1;
			
		}
		
		else {
			if ((int)data[3] == 255) {
//				std::cout << "r:" << (int)data[0] << ", g:" << (int)data[1] << ", b:" << (int)data[2] << ", a:" << (int)data[3] << std::endl;
//				std::cout << "EdgeID : " <<pickedID << std::endl;
//				std::vector<int>::iterator it;
					
				bool exists = false;
				for (int i=0; i<pickedIDs.size(); i++) {
//					if ( pickedIDs[i] == pickedID-0x00ff0000) {
					if ( pickedIDs[i] == pickedID) {
						pickedIDs.erase(pickedIDs.begin()+i);
						exists = true;
						break;
					}
				}
					
//				it = std::find(pickedIDs.begin(), pickedIDs.end(), pickedID);
//				if (it != pickedIDs.end())
//					pickedIDs.erase(pickedIDs.begin()+*it);
//				else
//					pickedIDs.push_back(pickedID);
				if (!exists)
					pickedIDs.push_back(pickedID);
			}
		}
//		for (int i : pickedIDs)
//			std::cout << i << " " ;
//		std::cout<< std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		lefthold = false;
//		oldxclick=thetaX;
//		oldyclick=thetaY;

	}
//	std::cout<< xclick << std::endl;
	renderGeometries();
}

// Callback for mouse motion
void InputHandler::motion(GLFWwindow* window, double x, double y) {
/*	double x_raw, y_raw, x_ogl, y_ogl;//, dx0,dy0;
	if (lefthold) {	
		glfwGetCursorPos(window, &x_raw, &y_raw);
		x_ogl = (-256+x_raw)/256;
		y_ogl = (256-y_raw)/256;
		thetaX = oldxclick-x_ogl+xclick;
		thetaY = oldyclick+y_ogl-yclick;
		renderGeometries();
	}*/
	
	glFlush();
	glFinish();
//	glReadBuffer((GLenum)GL_COLOR_ATTACHMENT0_EXT);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unsigned char data[4];
//		int data[4];
	glReadPixels(x, 1024-y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	pickedID = ((data[0] + data[1]*256 + data[2]*256*256)) &0x00ffffff;
	if (pickedID == 0x00ffffff) {
//			std::cout << pickedID << ": background" << std::endl;
//			lefthold = true;
//		pickedID = -1;
	}
	else {
		if ((int)data[3] == 255) {
//			std::cout << "r:" << (int)data[0] << ", g:" << (int)data[1] << ", b:" << (int)data[2] << ", a:" << (int)data[3] << std::endl;
//			std::cout << "EdgeID : " <<pickedID << std::endl;
		}
	}
	
	glm::vec2 newPos = glm::vec2(x/256, -y/256)*2.f - glm::vec2(1.f);
	glm::vec2 diff = newPos - mousePos;
	float dt = diff.x;
	float dp = diff.y;
		
	if (lefthold) {	
//		glm::vec3 oldCamDir = camDir;
		cameraController(glm::vec3(-dp, dt, 0.0f));
		renderGeometries();
	}	
	mousePos = newPos;
}


// Callback for mouse scroll
void InputHandler::scroll(GLFWwindow* window, double x, double y) {
	cameraController(glm::vec3(0.0f, 0.0f, y));
	renderGeometries();
}

// Callback for window reshape/resize
void InputHandler::reshape(GLFWwindow* window, int width, int height) {
	renderEngine->setWindowSize(width, height);
//	renderGeometries();

}

void InputHandler::controller() {
	if (moveLeft)
		camPos -= glm::vec3(sin(thetaX-M_PI/2.0f),0,cos(thetaX-M_PI/2.0f))*0.01f;
	if (moveRight)
		camPos += glm::vec3(sin(thetaX-M_PI/2.0f),0,cos(thetaX-M_PI/2.0f))*0.01f;
	if (moveBack)
		camPos -= glm::vec3(	cos(-thetaY) * sin(thetaX), sin(-thetaY),cos(thetaY) * cos(thetaX))*0.01f;
	if (moveForward)
		camPos += glm::vec3(	cos(-thetaY) * sin(thetaX), sin(-thetaY),cos(thetaY) * cos(thetaX))*0.01f;
	if (moveUp)
		camPos += glm::vec3(0.0f,1.0f,0.0f)*0.01f;
	if (moveDown)
		camPos -= glm::vec3(0.0f,1.0f,0.0f)*0.01f;
}
void InputHandler::cameraController(glm::vec3 dPos) {
		camSpherePos -= dPos;
	float theta = camSpherePos.y;
	float phi = camSpherePos.x;
	if (phi >M_PI-0.001) phi = M_PI-0.001;
	if (phi <0.001) phi = 0.001f;
	camPos = camSpherePos.z * glm::vec3(cos(theta) * sin(phi),
						sin(theta) * sin(phi),
						cos(phi)) + camTarget;
	camDir = glm::normalize(camPos - camTarget);
	camRight = glm::normalize(glm::cross(camDir, glm::vec3(0, 0, 1)));
	camUp =  glm::normalize(glm::cross(camRight, camDir));
}

void InputHandler::renderGeometries() {
//	controller();
//	std::cout << camPos.x << "," << camPos.y << "," << camPos.z << std::endl;
	glm::vec3 direction(cos(thetaY)*sin(thetaX),sin(thetaY),cos(thetaY)*cos(thetaX));
	glm::vec3 right = glm::vec3(sin(thetaX-M_PI/2.0f),0,cos(thetaX-M_PI/2.0f));
//	glm::mat4 proj = glm::lookAt(camPos, camPos+direction, glm::cross(right,direction));
	glm::mat4 proj = glm::lookAt(camPos, camTarget, camUp);
	glm::mat4 view = glm::perspective(glm::radians(60.0f), 3.0f / 3.0f, 0.1f, 1024.0f);
//	glm::mat4 view = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);

//	renderEngine->render(stuff, proj, glm::mat4(1.0), 1);
//	renderEngine->render(stuff, glm::mat4(1.0), 1);
//	stuff.clear();
/*	Geometry cube;
	cube.makeCube(glm::vec3(0.0f), .5f, .5f, .5f);
	renderEngine->assignBuffers(cube);
	renderEngine->updateBuffers(cube);
	stuff.push_back(&cube);*/
//	glDisable(GL_MULTISAMPLE);  
	for (Geometry* g : stuff) {
		g->unhighlightEdge();
		for(int i : pickedIDs) {
			if (i >= 0) {
				g->elevateFace(i, elevate);
				g->highlightEdge(i);
			}
		}
//		renderEngine->assignBuffers(*g);
		renderEngine->updateBuffers(*g);
	}
	renderEngine->render(stuff, view*proj, 0);
//	renderEngine->render(stuff, glm::mat4(1.0), 1);
}
