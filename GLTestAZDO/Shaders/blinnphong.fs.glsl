#version 410 core

// Output
layout (location = 0) out vec4 color;

// Input from vertex shader
in VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

// Material properties
uniform vec3 Kd = vec3(0.5, 0.2, 0.7);
uniform vec3 Ks = vec3(0.7);
uniform float m = 200.0;

void main(void)
{
    // Normalize the incoming N, L and V vectors
    vec3 n = normalize(fs_in.N);
    vec3 l = normalize(fs_in.L);
    vec3 v = normalize(fs_in.V);
    vec3 h = normalize(l + v);
	vec3 Lo = vec3(0.0);
    // Compute the diffuse and specular components for each fragment
	float nDotL = clamp(dot(n, l), 0.0, 1.0);
	float nDotH = clamp(dot(n, h), 0.0, 1.0);
	Lo += (Kd + Ks * pow(nDotH, m)) * nDotL;

    // Write final color to the framebuffer
	color = vec4(Lo, 1.0);
}

