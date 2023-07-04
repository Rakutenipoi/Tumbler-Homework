#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	color = in_color;
	gl_Position = projection * view * model * vec4(in_position, 1.0);
}



