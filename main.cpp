#include<iostream>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
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
constexpr unsigned int width = 1900;
constexpr unsigned int height = 1900;

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

struct LineRenderer
{
	FunctionPath line;
	VAO VAO2;
	VBO VBO2;
	bool calculateTriggered = false;
	char userInput[128] = "";
	float functionColor[3] = { 0.6f, 0.0f, 0.0f };

	LineRenderer(unsigned int width) : line(width) {}

	// Calculate vertices
	void calculate(const char* fn, float userZoom){
		line.parseFunction(fn);
		line.makeVertices(userZoom);

		VAO2.Bind();
		VBO2 = VBO(line.vertices.data(), line.vertices.size() * sizeof(float));
		VAO2.LinkAttrib(VBO2, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
		VAO2.Unbind();
	}

	// Draw the line
	void draw(GLint& colorFunctionLoc){
		VAO2.Bind();
		glPointSize(5.0f);
		glUniform3fv(colorFunctionLoc, 1, glm::value_ptr(glm::vec3(functionColor[0], functionColor[1], functionColor[2])));
		glDrawArrays(GL_POINTS, 0, line.vertices.size() / 2);
		VAO2.Unbind();
	}

	void imguiInput(unsigned int& userZoom) {
		ImGui::Text("Input a function:");
		ImGui::InputText("", userInput, IM_ARRAYSIZE(userInput));
		if (ImGui::Button("calculate") || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			std::cout << "Calculate: " << userInput << "\n";
			calculateTriggered = true;
		}
		if (calculateTriggered) {
			ImGui::Text("Data sent");
			calculate(userInput, userZoom);
			calculateTriggered = false; // Reset flag & input
			userInput[0] = '\0';
		}
		ImGui::ColorEdit3("Line Color", functionColor);
	}
};

int main()
{
	Setup VIEWPORT("Graph", width, height, 0.7f, 0.7f, 0.7f);
	VIEWPORT.glfwSetup();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(VIEWPORT.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().FontGlobalScale = 2.5f;
	unsigned int userZoom = 5;

	// Shader objects
	Shader Grid("default.vert", "default.frag");
	Shader function("function.vert", "default.frag");

	// Grid line VAO
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


	// Function VAO
	std::vector<std::unique_ptr<LineRenderer>> lines;
	
	int linesNum = 3;
	for (int i = 0; i < linesNum; i++) {
		lines.emplace_back(std::make_unique<LineRenderer>(width));
	}
	GLint colorFunctionLoc = glGetUniformLocation(function.ID, "Color");

	

	while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
	{
		VIEWPORT.glClearCurrentColor();
		glClear(GL_COLOR_BUFFER_BIT);

		// ImGui initializeation
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


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
		VAO1.Unbind();

		// Draw functions
		function.Activate();
		for (auto& line : lines) {
			line->draw(colorFunctionLoc);
		}
		

		// ImGUI window creation
		ImGui::Begin("Function input");
		
			//ImGui::ShowDemoWindow();
			for (int i = 0; i < lines.size(); i++) {
				ImGui::PushID(i);
				lines[i]->imguiInput(userZoom);
				ImGui::PopID();
			}

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(VIEWPORT.getWindow());
		glfwPollEvents();
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	Grid.Delete();
	return 0;
}