#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;
layout (location = 0) out vec2 passedTextureCoordinates;

void main() {

	gl_Position = vec4(aPosition, 1.0f);
	passedTextureCoordinates = aTextureCoordinates;
}