#ifndef FUNCTIONPATH
#define FUNCTIONPATH

#include<iostream>
#include"exprtk/exprtk.hpp"
#include<glm/glm.hpp>
#include<glad/glad.h>

class FunctionPath {
private:
	std::string m_function = "";
	unsigned int m_width;
	typedef exprtk::symbol_table<float> symbol_table_t;
	typedef exprtk::expression<float>   expression_t;
	typedef exprtk::parser<float>       parser_t;
	symbol_table_t symbol_table;
	expression_t expression;
	parser_t parser;
	GLfloat x = 0.0f;
public:
	std::vector<GLfloat> vertices;
	FunctionPath(unsigned int width);
	void computeValues();
	void parseFunction(std::string function);
	void makeVertices(int zoom);
	~FunctionPath();
};

#endif
