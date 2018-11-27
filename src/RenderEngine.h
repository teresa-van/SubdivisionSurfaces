#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Geometry.h"
#include "ShaderTools.h"

class RenderEngine {

public:
	RenderEngine(GLFWwindow* window);

//	void render(const std::vector<Geometry*>& objects, glm::mat4 view, glm::vec3 pos, int z);
	void render(const std::vector<Geometry*>& objects, glm::mat4 view, int z);
	void assignBuffers(Geometry& object);
	void updateBuffers(Geometry& object);
	void deleteBuffers(Geometry& object);
	void setWindowSize(int width, int height);
	
	void initializeFBO();

private:
	GLFWwindow* window;

	GLuint mainProgram;

	glm::mat4 ortho;
	
	Geometry *fbogeo;
	GLuint fbo = 0;
	GLuint fbo0 = 1;
	GLuint fbo_vao;
	GLuint renderedTexture;
	GLuint renderedTexture0;
	GLuint depthBuffer;
	GLuint depthRenderBuffer;
};

