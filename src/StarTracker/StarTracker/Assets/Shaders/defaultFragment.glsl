#version 460 core
layout (location = 0) out vec4 fragmentColor;

in vec4 passedColor;

void main() {

	fragmentColor = passedColor;
}