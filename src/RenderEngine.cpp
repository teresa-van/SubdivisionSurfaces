#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) : window(window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float aspectRatio = ((float)width / (float)height);
	ortho = glm::ortho(-256.0f * aspectRatio, 256.0f * aspectRatio, -256.0f, 256.0f, -256.0f, 256.0f);

	mainProgram = ShaderTools::compileShaders("shaders/main.vert", "shaders/main.frag");
	initializeFBO();

	// Set OpenGL state
	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);

	// glEnable(GL_LINE_SMOOTH);
	glPointSize(5.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0);

}

// Called to render provided objects under view matrix
//void RenderEngine::render(const std::vector<Geometry*>& objects, glm::mat4 view, glm::vec3 pos, int z) {
void RenderEngine::render(const std::vector<Geometry*>& objects, glm::mat4 view, int z) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(mainProgram);

	glUniform1i(glGetUniformLocation(mainProgram, "ourTexture"), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, renderedTexture, 0);

	// glActiveTexture(GL_TEXTURE0 + 0);
	// glBindTexture(GL_TEXTURE_2D, renderedTexture);
//std::cout << "dfasdfasdfsadfsa" << std::endl;
	for (const Geometry* o : objects) {

glm::mat4 modelView = view * o->modelMatrix;
		// DRAW TO FB
		// glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glUniform1i(glGetUniformLocation(mainProgram, "depth"), z);
		glUniform1i(glGetUniformLocation(mainProgram, "toTexture"), 0);

		// glUniform3fv(glGetUniformLocation(mainProgram, "position"), 1, glm::value_ptr(pos));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
		glBindVertexArray(o->vao);
		glDrawArrays(o->drawMode, 0, o->verts.size());

// 		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
// 		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
// 		glBindTexture(GL_TEXTURE_2D, renderedTexture);
//
// //		glm::mat4 modelView = view * o->modelMatrix;
// 		glUniform1i(glGetUniformLocation(mainProgram, "depth"), z);
// //		glUniform3fv(glGetUniformLocation(mainProgram, "position"), 1, glm::value_ptr(pos));
// 		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
// 		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
// 		glDrawArrays(GL_TRIANGLES, 0, 6);
// 		glBindVertexArray(0);
// 		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Get render data back to CPU
		std::vector<int> pixelsVec(1024*1024);
		pixels = pixelsVec.data();
		// int numBytes = 1024 * 1024 * 4;
		// size_t memSize = numBytes * sizeof(GLubyte);
		// pixels = new GLubyte[numBytes];

		// Bind the texture and grab the data
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB_INTEGER, GL_INT, pixels);

		// Have the texture in main memory, can delete from GPU now
		glDeleteTextures(1, &renderedTexture);
		glDeleteFramebuffers(1, &fbo);

		glUniform1i(glGetUniformLocation(mainProgram, "depth"), z);
		glUniform1i(glGetUniformLocation(mainProgram, "toTexture"), 1);

		// glUniform3fv(glGetUniformLocation(mainProgram, "position"), 1, glm::value_ptr(pos));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
		glBindVertexArray(o->vao);
		glDrawArrays(o->drawMode, 0, o->verts.size());

		glBindVertexArray(0);
		glUseProgram(0);

/*		glBindVertexArray(o->vao);
		glm::mat4 modelView = view * o->modelMatrix;
		glUniform1i(glGetUniformLocation(mainProgram, "depth"), z);
//		glUniform3fv(glGetUniformLocation(mainProgram, "position"), 1, glm::value_ptr(pos));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
		glDrawArrays(o->drawMode, 0, o->verts.size());
		glBindVertexArray(0);*/
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
/*		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glUseProgram(mainProgram);
		glBindVertexArray(o->vao);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glDrawArrays(o->drawMode, 0, o->verts.size());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
/*		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindVertexArray(o->vao);
		glDrawArrays(o->drawMode, 0, o->verts.size());
		glBindVertexArray(o->vao);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
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

	glGenBuffers(1, &object.textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, object.textureBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

// Updates geometry in buffer
void RenderEngine::updateBuffers(Geometry& object) {
	// Updates data in buffer
	glBindBuffer(GL_ARRAY_BUFFER, object.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.verts.size(), object.verts.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, object.colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.colours.size(), object.colours.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, object.textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * object.uvs.size(), object.uvs.data(), GL_STATIC_DRAW);

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

/*	glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
//    GLuint depthRenderbuffer;
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1024, 1024);

glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);*/
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

//	std::vector<glm::vec2> sp = {
//		glm::vec2(0.0f,0.0f), glm::vec2(0.0f,1024.0f), glm::vec2(1024.0f,0.0f),
//		glm::vec2(1024.0f,0.0f), glm::vec2(0.0f,1024.0f), glm::vec2(1024.0f,1024.0f)
//	fbogeo->makeFBO(sp);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);

// Set the list of draw buffers.
//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//glDrawBuffers(1, DrawBuffers);
}
