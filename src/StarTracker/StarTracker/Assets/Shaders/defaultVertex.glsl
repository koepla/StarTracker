#version 460 core
layout (location = 0) in vec3 aPosition;

out vec4 passedColor;

uniform mat4 uTransform;

void main() {

	gl_Position = uTransform * vec4(aPosition, 1.0f);
	passedColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}