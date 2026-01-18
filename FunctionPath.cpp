#include "FunctionPath.h"

FunctionPath::FunctionPath(unsigned int width) : m_width(width){}


void FunctionPath::parseFunction(std::string function) {

    const std::string expression_string = function;

    symbol_table.add_variable("x", x);
    symbol_table.add_constants();

    expression.register_symbol_table(symbol_table);
    parser.compile(expression_string, expression);

    //computeValues();
}

void FunctionPath::computeValues() {
    for (x = -1.0f; x <= 1.0f; x += 0.001f) {
        const float y = expression.value();
        printf("%19.15f\t%19.15f\n", x, y);
    }
}

void FunctionPath::makeVertices(int zoom) {
    float step = 2.0f / (m_width - 1);
    vertices.clear();
    vertices.reserve(m_width);
    for (x = -zoom; x <= zoom; x += step) {
        GLfloat y = static_cast<GLfloat>(expression.value());
        vertices.push_back(x / zoom);
        vertices.push_back(y / zoom);
    }
}

FunctionPath::~FunctionPath() {

}