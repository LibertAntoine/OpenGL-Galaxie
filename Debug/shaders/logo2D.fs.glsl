#version 330 core

in vec2 vFragTexture;
uniform sampler2D uTexture;

out vec3 fFragTexture;

void main() {
	vec4 tex = texture(uTexture, vFragTexture);

	fFragTexture = vec3(tex.rgb);
}