#version 460 core
layout (location = 0) out vec4 fragmentColor;
layout (location = 0) in vec2 passedTextureCoordinates;

uniform sampler2D uTexture;

void main() {

	fragmentColor = texture(uTexture, passedTextureCoordinates);
}