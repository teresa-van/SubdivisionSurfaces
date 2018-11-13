#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) : window(window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float aspectRatio = ((float)width / (float)height);
	ortho = glm::ortho(-256.0f * aspectRatio, 256.0f * aspectRatio, -256.0f, 256.0f, -256.0f, 256.0f);

	mainProgram = ShaderTools::compileShaders("shaders/main.vert", "shaders/main.frag");

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
		glBindVertexArray(o->vao);
		
		glm::mat4 modelView = view * o->modelMatrix;
		glUniform1i(glGetUniformLocation(mainProgram, "depth"), z);
//		glUniform3fv(glGetUniformLocation(mainProgram, "position"), 1, glm::value_ptr(pos));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
		glUniformMatrix4fv(glGetUniformLocation(mainProgram, "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
//		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDrawArrays(o->drawMode, 0, o->verts.size());
		glBindVertexArray(0);
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

	glBindVertexArray(0);
}

// Updates geometry in buffer
void RenderEngine::updateBuffers(Geometry& object) {
	// Updates data in buffer
	glBindBuffer(GL_ARRAY_BUFFER, object.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.verts.size(), object.verts.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, object.colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.colours.size(), object.colours.data(), GL_DYNAMIC_DRAW);
}

// Deletes buffers
void RenderEngine::deleteBuffers(Geometry& object) {
	glDeleteBuffers(1, &object.vertexBuffer);
	glDeleteBuffers(1, &object.colourBuffer);
	glDeleteVertexArrays(1, &object.vao);
}

// Sets projection and viewport for new width and height
void RenderEngine::setWindowSize(int width, int height) {
	glViewport(0, 0, width, height);
	float aspectRatio = ((float)width / (float)height);
	ortho = glm::ortho(-256.0f * aspectRatio, 256.0f * aspectRatio, -256.0f, 256.0f, -256.0f, 256.0f);
}
