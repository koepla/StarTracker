#version 460 core
layout (location = 0) out vec4 fragmentColor;
layout (location = 0) in vec3 passedColor;
layout (location = 1) in vec2 passedTextureCoordinates;

uniform sampler2D uTexture;

void main() {

    fragmentColor = vec4(passedColor, 1.0f) * texture(uTexture, passedTextureCoordinates);
}