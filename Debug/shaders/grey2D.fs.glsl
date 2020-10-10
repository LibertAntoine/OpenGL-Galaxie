#version 330 core

in vec3 vFragColor;

out vec3 fFragColor;

void main() {
vec3 grey = vec3((vFragColor.r + vFragColor.g + vFragColor.b) /3);
fFragColor = grey;
}