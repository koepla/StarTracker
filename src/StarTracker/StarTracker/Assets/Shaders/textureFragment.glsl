#version 460 core
layout (location = 0) out vec4 fragmentColor;

in vec3 passedTexCoords;

uniform sampler2D textureSampler;

void main() {

	fragmentColor = texture(textureSampler, passedTexCoords.xy);
}