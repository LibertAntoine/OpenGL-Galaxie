#version 330 core

in vec3 vFragTexture;
uniform vec3 uColor;

out vec3 fFragTexture;

void main() {

	fFragTexture = uColor;
}