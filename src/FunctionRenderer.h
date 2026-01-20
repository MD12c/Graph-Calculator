#ifndef FUNCTIONRENDERER
#define FUNCTIONRENDERER

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <iostream>
#include "FunctionPath.h"
#include "Graphics/VAO.h"
#include "Graphics/VBO.h"
#include "Graphics/EBO.h"

struct FunctionRenderer {
	FunctionPath function;
	VAO VAO2;
	VBO VBO2;
	bool calculateTriggered = false;
	char userInput[128] = "";
	float functionColor[3] = { 0.6f, 0.0f, 0.0f };

	FunctionRenderer(unsigned int width) : function(width) {}

	// Calculate vertices
	void calculate(const char* fn, float userZoom) {
		function.parseFunction(fn);
		function.makeVertices(userZoom);

		VAO2.Bind();
		VBO2 = VBO(function.vertices.data(), function.vertices.size() * sizeof(float));
		VAO2.LinkAttrib(VBO2, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
		VAO2.Unbind();
	}

	// Draw the function
	void draw(GLint& colorFunctionLoc) {
		VAO2.Bind();
		glPointSize(5.0f);
		glUniform3fv(colorFunctionLoc, 1, glm::value_ptr(glm::vec3(functionColor[0], functionColor[1], functionColor[2])));
		glDrawArrays(GL_POINTS, 0, function.vertices.size() / 2);
		VAO2.Unbind();
	}

	void imguiInput(unsigned int& userZoom) {
		ImGui::Text("Input a function:");
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("", userInput, IM_ARRAYSIZE(userInput));
		if (ImGui::Button("calculate", ImVec2(ImGui::GetContentRegionAvail().x, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			std::cout << "Calculate: " << userInput << "\n";
			calculateTriggered = true;
		}
		if (calculateTriggered) {
			ImGui::Text("Data sent");
			calculate(userInput, userZoom);
			calculateTriggered = false; // Reset flag & input
			userInput[0] = '\0';
		}
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::ColorEdit3("", functionColor);
	}
};

#endif

