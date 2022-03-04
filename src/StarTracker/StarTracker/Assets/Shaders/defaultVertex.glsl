#version 460 core
layout (location = 0) in vec3 aPosition;

out vec4 passedColor;

void main() {

	gl_Position = vec4(aPosition, 1.0f);
	passedColor = gl_Position + 0.5f;
}