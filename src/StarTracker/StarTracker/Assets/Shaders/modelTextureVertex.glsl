#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;

layout (location = 0) out vec3 passedPosition;
layout (location = 1) out vec3 passedNormal;
layout (location = 2) out vec2 passedTextureCoordinates;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform mat4 uTransform;

void main() {
	
	passedPosition = vec3(uModel * vec4(aPosition, 1.0f));
	passedNormal = mat3(transpose(inverse(uModel))) * aNormal;
	passedTextureCoordinates = aTextureCoordinates;
	gl_Position = uProjection * uView * vec4(aPosition, 1.0f);
}
