#version 330 core

in vec3 vFragColor;
in vec2 vVertexPosition;

out vec3 fFragColor;

vec2 complexSqr(vec2 z) {
	vec2 zOut = vec2(0.);
	zOut.x = z.x * z.x - z.y * z.y;
	zOut.y = 2 * z.x * z.y;
	return zOut;
}

void main() {
	int Nmax = 20;
	vec2 z = vVertexPosition;
	vec3 finalColor = vec3(0.);
	for(float i = 0; i <= Nmax; i++) {
		if(2 <= length(z)) {
			finalColor = vec3(i / Nmax, (i/2) / Nmax, (i/3) / Nmax); 
			break;
		}
		z = complexSqr(z) + z;
	}
	fFragColor = finalColor;
}