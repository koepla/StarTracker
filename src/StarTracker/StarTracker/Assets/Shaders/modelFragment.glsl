#version 460 core
layout (location = 0) out vec4 fragmentColor;

layout (location = 0) in vec3 passedPosition;
layout (location = 1) in vec3 passedNormal;

uniform vec3 uCameraPosition;
uniform vec3 uObjectColor;
uniform vec3 uLightColor;

void main() {

	float ambientStrength = 0.1f;
	vec3 ambientVector = ambientStrength * uLightColor;

	// diffuse
	vec3 normal = normalize(passedNormal);
	vec3 lightDirection = normalize(uCameraPosition - passedPosition);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuseVector = diffuse * uLightColor;

	// specular
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(uCameraPosition - passedPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
	vec3 specularVector = specularStrength * specular * uLightColor;

	vec3 result = (ambientVector + diffuseVector + specularVector) * uObjectColor;
	fragmentColor = vec4(result, 1.0f);
}