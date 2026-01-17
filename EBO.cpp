#include "EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size) {
	glGenBuffers(1, &ID); // generate 1 buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // bind the buffer to the GL_ARRAY_BUFFER target
	// GL_ARRAY_BUFFER is a big array that contains vertex attributes such as position, color, normal, etc.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); // copy the vertex data to the buffer
	//GL_ARRAY_BUFFER is the target to which the buffer is bound
	//sizeof(vertices) is the size of the data in bytes
	//vertices is a pointer to the data to be copied
	//GL_STATIC_DRAW is a usage hint that tells OpenGL how we intend to use the data (GL_STATIC_DRAW -> data will change very rarely)
		//other options are GL_DYNAMIC_DRAW and GL_STREAM_DRAW
		//GL_STATIC_DRAW means that the data will not change or will change very rarely

}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // bind the buffer to the GL_ARRAY_BUFFER target
	// GL_ARRAY_BUFFER is a big array that contains vertex attributes such as position, color, normal, etc.

}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // bind the buffer to the GL_ARRAY_BUFFER target
	// GL_ARRAY_BUFFER is a big array that contains vertex attributes such as position, color, normal, etc.

}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);

}