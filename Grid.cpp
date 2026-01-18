#include"Grid.h"

Grid::Grid(const char* vertex, const char* fragment) : GridShader(vertex, fragment), VBO1(gridLine, sizeof(gridLine)), EBO1(indices, sizeof(indices)) {
	GLint colorLoc = glGetUniformLocation(GridShader.ID, "Color");
	GLint modelLoc = glGetUniformLocation(GridShader.ID, "translated");
	VAO1.Bind();
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
}

void Grid::drawGrid() {
	GLfloat shade = 0.5f;
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(shade, shade, shade)));
	for (int i = 0; i < linesAmount; i++) {
		float offset = spacing + i * 0.1f - 1.0f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, offset, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}
	for (int i = 0; i < linesAmount; i++) {
		float offset = spacing + i * 0.1f - 1.0f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(offset, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}
}

void Grid::drawCenterlines() {
	GLfloat shade = 0.3f;
	glm::mat4 baseLineH = glm::mat4(1.0f);
	baseLineH = glm::scale(baseLineH, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(baseLineH));
	shade = 0.2f;
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(shade, shade, shade)));
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

	glm::mat4 baseLineV = glm::mat4(1.0f);
	baseLineV = glm::scale(baseLineV, glm::vec3(1.5f, 1.5f, 1.5f));
	baseLineV = glm::rotate(baseLineV, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(baseLineV));
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	VAO1.Unbind();
}

void Grid::BindGrid() {
	GridShader.Activate();
	VAO1.Bind();
	VBO1.Bind();
	EBO1.Bind();
}

void Grid::UnbindGrid() {
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
}

void Grid::deleteGrid() {
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	GridShader.Delete();
}