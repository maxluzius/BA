#version 330

layout (location = 0) in vec3 in_Position;
//Eingabe x ist die Tiefe, y ist die Höhe und z ist Breite.

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec4 position;
	
//uniform vec3 cameraPos;

void main() {	
	
	position = P * V * M * vec4(in_Position,1);

	gl_Position = P * V * M * vec4(in_Position,1);
}
