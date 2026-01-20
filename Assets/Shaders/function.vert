#version 330 core

layout (location = 0) in vec2 aPos;

out vec3 color;

uniform vec3 Color;
uniform mat4 translated;

void main(){
	gl_Position = vec4(aPos, 1.0f, 1.0f);
	color = Color;
}
