#version 460 core
layout (location = 0) out vec4 fragmentColor;
layout (location = 0) in vec2 passedTextureCoordinates;

uniform sampler2D uTexture;
uniform float[9] uKernel;

void main() {
	const float offset = 1.0f / 300.0f;
	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset), // top-left
		vec2( 0.0f,    offset), // top-center
		vec2( offset,  offset), // top-right
		vec2(-offset,  0.0f),   // center-left
		vec2( 0.0f,    0.0f),   // center-center
		vec2( offset,  0.0f),   // center-right
		vec2(-offset, -offset), // bottom-left
		vec2( 0.0f,   -offset), // bottom-center
		vec2( offset, -offset)  // bottom-right
	);

	vec3 textureSamples[9];
	for (int i = 0; i < 9; i++) {
		textureSamples[i] = vec3(texture(uTexture, passedTextureCoordinates + offsets[i]));
	}

	vec3 color = vec3(0.0f);
	for (int i = 0; i < 9; i++) {
		color += textureSamples[i] * uKernel[i];
	}

	fragmentColor = vec4(color, 1.0f);
}