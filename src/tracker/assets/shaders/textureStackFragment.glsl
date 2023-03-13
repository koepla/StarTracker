#version 460 core
layout (location = 0) out vec4 fragmentColor;
layout (location = 0) in vec2 passedTextureCoordinates;

uniform int uNumberOfPassedTextures;
uniform sampler2D uTextures[32];

void main() {
	vec4 result = vec4(0.0f);
	for (int i = 0; i < uNumberOfPassedTextures; i++) {
		result += texture(uTextures[i], passedTextureCoordinates);
	}

	float weight = 1.0f / float(uNumberOfPassedTextures);
	fragmentColor = result * weight;
}