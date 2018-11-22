#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) : window(window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float aspectRatio = ((float)width / (float)height);
	ortho = glm::ortho(-256.0f * aspectRatio, 256.0f * aspectRatio, -256.0f, 256.0f, -256.0f, 256.0f);

	mainProgram = ShaderTools::compileShaders("shaders/main.vert", "shaders/main.frag");
//	initializeFBO();

	// Set OpenGL state
	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
	
	glEnable(GL_LINE_SMOOTH);
	glPointSize(5.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0);
}

// Called to render provided objects under view matrix
//void RenderEngine::render(const std::vector<Geometry*>& objects, glm::mat4 view, glm::vec3 pos, int z) {
void RenderEngine::render(const std::vector<Geometry*>& objects, glm::mat4 view, int z) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(mainProgram);

	for (const Geometry* o : objects) {
		glm::mat4 modelView = view * o->modelMatrix;
		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
//		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glUniform1i(glGetUniformLocation(mainProgram, "depth"), z);
//		glUniform3fv(glGetUniformLocation(mainProgram, "position"), 1, glm::value_ptr(pos));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
		glBindVertexArray(fbogeo->vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
/*
		glBindVertexArray(o->vao);
//		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glm::mat4 modelView = view * o->modelMatrix;
		glUniform1i(glGetUniformLocation(mainProgram, "depth"), z);
//		glUniform3fv(glGetUniformLocation(mainProgram, "position"), 1, glm::value_ptr(pos));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
		glDrawArrays(o->drawMode, 0, o->verts.size());
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);*/
	}
}

// Assigns and binds buffers
void RenderEngine::assignBuffers(Geometry& object) {
	// Bind attribute array for triangles
	glGenVertexArrays(1, &object.vao);
	glBindVertexArray(object.vao);

	// Vertex buffer
	glGenBuffers(1, &object.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, object.vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	// Colour buffer
	glGenBuffers(1, &object.colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, object.colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	
/*	glGenBuffers(1, &object.colourBuffer0);
	glBindBuffer(GL_ARRAY_BUFFER, object.colourBuffer0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);
*/
	glBindVertexArray(0);
}

// Updates geometry in buffer
void RenderEngine::updateBuffers(Geometry& object) {
	// Updates data in buffer
	glBindBuffer(GL_ARRAY_BUFFER, object.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.verts.size(), object.verts.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, object.colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.colours.size(), object.colours.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_FRAMEBUFFER, object.colourBuffer);
//	glBufferData(GL_FRAMEBUFFER, sizeof(glm::vec3) * object.colours.size(), object.colours.data(), GL_DYNAMIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, object.colourBuffer0);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.colours0.size(), object.colours0.data(), GL_DYNAMIC_DRAW);
	
	
}

// Deletes buffers
void RenderEngine::deleteBuffers(Geometry& object) {
	glDeleteBuffers(1, &object.vertexBuffer);
	glDeleteBuffers(1, &object.colourBuffer);
//	glDeleteBuffers(1, &object.colourBuffer0);
	glDeleteVertexArrays(1, &object.vao);
}

// Sets projection and viewport for new width and height
void RenderEngine::setWindowSize(int width, int height) {
	glViewport(0, 0, width, height);
	float aspectRatio = ((float)width / (float)height);
	ortho = glm::ortho(-256.0f * aspectRatio, 256.0f * aspectRatio, -256.0f, 256.0f, -256.0f, 256.0f);
}

void RenderEngine::initializeFBO() {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glGenTextures(1, &renderedTexture);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 1024, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
    
    std::vector<glm::vec2> sp = {
		glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1024.0f, 0.0f), glm::vec3(1024.0f, 0.0f, 0.0f), 
		glm::vec3(1024.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1024.0f, 0.0f), glm::vec3(1024.0f, 1024.0f, 0.0f) };
	fbogeo->makeFBO(sp);
// Set the list of draw buffers.
//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//glDrawBuffers(1, DrawBuffers);
}
