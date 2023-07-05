#version 330 core

uniform vec3 color;
in vec3 col;
out vec4 fragColor;

void main() {
	
	fragColor = vec4((col + 1) / 2, 1.0);
}
