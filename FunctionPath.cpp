#include "FunctionPath.h"

FunctionPath::FunctionPath(std::string function, unsigned int width) : m_function(function), m_width(width){}


void FunctionPath::parseFunction() {

    const std::string expression_string = m_function;

    symbol_table.add_variable("x", x);
    symbol_table.add_constants();

    expression.register_symbol_table(symbol_table);
    parser.compile(expression_string, expression);

    computeValues();
}

void FunctionPath::computeValues() {
    for (x = -1.0f; x <= 1.0f; x += 0.001f) {
        const float y = expression.value();
        printf("%19.15f\t%19.15f\n", x, y);
    }
}

void FunctionPath::makeVertices() {
    float step = 2.0f / (m_width - 1);
    vertices.reserve(m_width);
    for (x = -1.0f; x <= 1.0f; x += step) {
        float y = expression.value();
        vertices.push_back(glm::vec2(x, y));
    }
}

FunctionPath::~FunctionPath() {

}