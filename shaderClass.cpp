#include"shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//vertex & fragment shader obj creation + compilation + linking into a global shader program + deletion of shader objects
	//create shader objects + compile + link
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // create vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL); // attach shader source code to the shader object named vertexShader
	glCompileShader(vertexShader); // compile vertex shader into machine code
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL); // attach shader source code to the shader object named fragmentShader
	glCompileShader(fragmentShader); // compile fragment shader into machine code



	// create global shader program + link both shaders into it
	ID = glCreateProgram(); // create shader program object
	glAttachShader(ID, vertexShader); // attach vertex shader to the shader builtin program
	glAttachShader(ID, fragmentShader); // attach fragment shader to the shader builtin program
	glLinkProgram(ID); // link the shader program

	glDeleteShader(vertexShader); // delete the vertex shader object
	glDeleteShader(fragmentShader); // delete the fragment shader object


}
	void Shader::Activate() {
		glUseProgram(ID); // use the shader program
	}
	void Shader::Delete() {
		glDeleteProgram(ID); // delete the shader program
	}