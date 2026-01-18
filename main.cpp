#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<math.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>

#include"Setup.h"
#include"ShaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"FunctionPath.h"
constexpr unsigned int width = 1500;
constexpr unsigned int height = 1500;

GLfloat gridLine[] =
{
	-1.0f, 0.002f, 0.0f,
	 1.0f, 0.002f, 0.0f,
	-1.0f,-0.002f, 0.0f,
	 1.0f,-0.002f, 0.0f

};

GLuint indices[] =
{
	0, 1, 2,
	2, 3, 1
};

void drawGrid() {

}

int main()
{
	Setup VIEWPORT("Graph", width, height, 0.7f, 0.7f, 0.7f);
	VIEWPORT.glfwSetup();


	Shader Grid("default.vert", "default.frag");
	Shader function("function.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(gridLine, sizeof(gridLine));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	GLint colorLoc = glGetUniformLocation(Grid.ID, "Color");
	GLint modelLoc = glGetUniformLocation(Grid.ID, "translated");



	FunctionPath line("sin(x)", width);
	line.parseFunction();
	line.makeVertices(10);
	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(line.vertices.data(), line.vertices.size() * sizeof(line.vertices[0]));
	VAO2.LinkAttrib(VBO2, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
	VAO2.Unbind();
	VBO2.Unbind();
	

	while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
	{
		VIEWPORT.glClearCurrentColor();
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw grid
		Grid.Activate();
		VAO1.Bind();

		GLfloat shade = 0.5f;
		glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(shade, shade, shade)));

		GLfloat spacing = 0.1f;
		int linesAmount = 19;
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


		// Draw centerlines 
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


		// Draw functions
		function.Activate();
		VAO2.Bind();
		glPointSize(5.0f);
		shade = 0.0f;
		glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(shade, shade, shade)));
		glDrawArrays(GL_POINTS, 0, line.vertices.size() / 2); // divide by 2 because each vertex has x and y
		





		glfwSwapBuffers(VIEWPORT.getWindow());
		glfwPollEvents();
	}
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	Grid.Delete();
	return 0;
}