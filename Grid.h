#ifndef GRID
#define GRID

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include <string>
#include <fstream>
#include"ShaderClass.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


class Grid {
private:
	GLfloat gridLine[12] = {
		-1.0f, 0.002f, 0.0f,
		 1.0f, 0.002f, 0.0f,
		-1.0f,-0.002f, 0.0f,
		 1.0f,-0.002f, 0.0f

	};
	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 1
	};
	Shader GridShader;
	VAO VAO1;
	VBO VBO1;
	EBO EBO1;
	GLfloat spacing = 0.1f;
	int linesAmount = 19;
	GLint colorLoc = glGetUniformLocation(GridShader.ID, "Color");
	GLint modelLoc = glGetUniformLocation(GridShader.ID, "translated");

public:
	Grid(const char* vertex, const char* fragment);
	void BindGrid();
	void drawGrid();
	void drawCenterlines();
	void UnbindGrid();
	void deleteGrid();

};

#endif

