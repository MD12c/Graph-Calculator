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
#include"Grid.h"
constexpr unsigned int width = 1900;
constexpr unsigned int height = 1900;

struct FunctionRenderer
{
	FunctionPath function;
	VAO VAO2;
	VBO VBO2;
	bool calculateTriggered = false;
	char userInput[128] = "";
	float functionColor[3] = { 0.6f, 0.0f, 0.0f };

	FunctionRenderer(unsigned int width) : function(width) {}

	// Calculate vertices
	void calculate(const char* fn, float userZoom){
		function.parseFunction(fn);
		function.makeVertices(userZoom);

		VAO2.Bind();
		VBO2 = VBO(function.vertices.data(), function.vertices.size() * sizeof(float));
		VAO2.LinkAttrib(VBO2, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
		VAO2.Unbind();
	}

	// Draw the function
	void draw(GLint& colorFunctionLoc){
		VAO2.Bind();
		glPointSize(5.0f);
		glUniform3fv(colorFunctionLoc, 1, glm::value_ptr(glm::vec3(functionColor[0], functionColor[1], functionColor[2])));
		glDrawArrays(GL_POINTS, 0, function.vertices.size() / 2);
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
	// Created GLFW context with glad.c implemented
	// Name of the window, width & height of the window, background color RGB
	Setup VIEWPORT("Graph", width, height, 0.7f, 0.7f, 0.7f);
	VIEWPORT.glfwSetup();

	// Created ImGUI context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(VIEWPORT.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().FontGlobalScale = 2.5f;
	unsigned int userZoom = 5;
	int temp = static_cast<int>(userZoom);

	// Shader objects
	Shader function("function.vert", "default.frag");

	// Function VAO
	std::vector<std::unique_ptr<FunctionRenderer>> functions;
	
	int functionsNum = 3;
	for (int i = 0; i < functionsNum; i++) {
		functions.emplace_back(std::make_unique<FunctionRenderer>(width));
	}
	GLint colorFunctionLoc = glGetUniformLocation(function.ID, "Color");

	// Make Grid
	Grid grid("default.vert", "default.frag");

	while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
	{
		VIEWPORT.glClearCurrentColor();
		glClear(GL_COLOR_BUFFER_BIT);

		// ImGui initializeation
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Draw Grid
		grid.BindGrid();
		grid.drawGrid();
		grid.drawCenterlines();
		grid.UnbindGrid();

		// Draw functions
		function.Activate();
		for (auto& function : functions) {
			function->draw(colorFunctionLoc);
		}
		
		// ImGUI window creation
		ImGui::Begin("Function input");
		
			//ImGui::ShowDemoWindow();
			if (ImGui::SliderInt("Zoom", &temp, 1, 10)) {
				userZoom = static_cast<unsigned int>(temp);
			}

			for (int i = 0; i < functions.size(); i++) {
				ImGui::PushID(i);
				functions[i]->imguiInput(userZoom);
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
	grid.deleteGrid();
	return 0;
}