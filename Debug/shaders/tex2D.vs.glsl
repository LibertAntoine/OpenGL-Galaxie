#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexTexture;
uniform float uTime;
uniform mat3 uModelMatrix;
uniform mat3 uModelMatrix2;
uniform mat3 uModelMatrix3;

out vec3 vFragTexture;

mat3 translate(float tx, float ty) {
	return mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
}

mat3 scale(float sx, float sy) {
	return mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
}

mat3 rotate(float a) {
	return mat3(vec3(cos(a),sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));
}
// La rotation déforme le triangle : 


void main() {
  vFragTexture = aVertexTexture;
  vec2 transformed = vec2(
  uModelMatrix3 * 
  //uModelMatrix * 
  uModelMatrix2 * 
  scale(0.3, 0.3) *
  vec3(aVertexPosition, 1)).xy;
  gl_Position = vec4(transformed, 0, 1);
};
