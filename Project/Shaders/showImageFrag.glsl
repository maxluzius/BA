#version 330

in vec2 passUV;

uniform sampler2D tex;

out vec3 fragmentColor;

void main() {
	vec3 color = texture(tex, passUV).xyz;
	fragmentColor = color;
}