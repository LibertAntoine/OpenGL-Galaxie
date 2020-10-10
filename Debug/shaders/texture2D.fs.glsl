#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec3 fFragColor;

void main() {
	float a = 15;
	float b = 60;
	//vec3 grey = vec3((vFragColor.r + vFragColor.g + vFragColor.b) /3);
	//float d = distance(vVertexPosition, vec2(0.,-0.20));
	//vec3 particule = vFragColor * a * exp(-b * d * d);

	fFragColor = particule;
}