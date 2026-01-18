#ifndef FUNCTIONPATH
#define FUNCTIONPATH

#include<iostream>
#include"exprtk/exprtk.hpp"
#include<glm/glm.hpp>

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
	float x = 0.0f;
public:
	std::vector<glm::vec2> vertices;
	FunctionPath(std::string function, unsigned int width);
	void computeValues();
	void parseFunction();
	void makeVertices();
	~FunctionPath();
};

#endif
