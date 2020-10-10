#version 330 core

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

uniform sampler2D uTexture;
uniform sampler2D uTexture2;

out vec3 fFragTexture;

vec3 blinnPhong() 
{
	return vec3(
		uLightIntensity * (
		uKd * (dot(uLightDir_vs,vNormal_vs)) 
		+ uKs * pow(dot(normalize(-vPosition_vs), vNormal_vs), uShininess)
		)
	);
}


void main() {
	vec4 tex = texture(uTexture, vTexCoords);
	vec4 tex2 = texture(uTexture2, vTexCoords);
	fFragTexture = (vec3(tex).xyz + vec3(tex2).xyz) * blinnPhong();
}