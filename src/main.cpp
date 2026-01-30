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

#include"Graphics/Window.h"
#include"Graphics/ShaderClass.h"
#include"Graphics/VAO.h"
#include"Graphics/VBO.h"
#include"Graphics/EBO.h"

#include"FunctionPath.h"
#include "FunctionRenderer.h"
#include"Grid.h"
constexpr unsigned int width = 1900;
constexpr unsigned int height = 1900;
// Width and height of the app
// Be shure that the two stay the same

int main() {
	// Created GLFW context with glad.c implemented
	// Name of the window, width & height of the window, background color RGB
	Window VIEWPORT("Graph", width, height, 0.7f, 0.7f, 0.7f);
	VIEWPORT.glfwSetup();


	// Created ImGUI context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/DejaVuSans.ttf", 16.0f);
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(VIEWPORT.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().FontGlobalScale = 2.5f;
	unsigned int userZoom = 5;
	auto userZoomPrev = userZoom;
	unsigned int userFunctionsNum = 3;
	int tempZoom = static_cast<int>(userZoom);
	int tempFunctions = static_cast<int>(userFunctionsNum);


	// Make Function VAO & Shaders
	Shader function("Assets/Shaders/function.vert", "Assets/Shaders/default.frag");
	std::vector<std::unique_ptr<FunctionRenderer>> functions;
	for (int i = 0; i < userFunctionsNum; i++) {
		functions.emplace_back(std::make_unique<FunctionRenderer>(width));
	}
	GLint colorFunctionLoc = glGetUniformLocation(function.ID, "Color");


	// Make Grid VAO & Shaders
	Grid grid("Assets/Shaders/default.vert", "Assets/Shaders/default.frag");


	while (!glfwWindowShouldClose(VIEWPORT.getWindow())) {
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

		if (userZoomPrev != userZoom) {
			for (auto& function : functions) {
				function->calculate(function->userInput, userZoom);
				userZoomPrev = userZoom;
			}
		}
		
		// ImGUI window
		ImGui::Begin("Function input");
		
			//ImGui::ShowDemoWindow();
			float widthButton = (ImGui::GetContentRegionAvail().x - 10.0f) * 0.5f;
			ImGui::Text("Zoom:");
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::SliderInt("##1", &tempZoom, 1, 10)) {
				userZoom = static_cast<unsigned int>(tempZoom);
			}
			if (ImGui::Button("Add Function##1", ImVec2(widthButton, 0))) {
				userFunctionsNum++;
				functions.emplace_back(std::make_unique<FunctionRenderer>(width));
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove Function##2", ImVec2(widthButton, 0)) && userFunctionsNum != 0) {
				userFunctionsNum--;
				functions.pop_back();
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