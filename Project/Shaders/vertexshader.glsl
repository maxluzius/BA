#version 330

layout (location = 0) in vec3 in_Position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

//uniform vec3 cameraPos;

void main() {	
	gl_Position = P * V * M * vec4(in_Position,1);
}