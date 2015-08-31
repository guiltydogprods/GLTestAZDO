#version 450 core
#extension GL_ARB_shader_draw_parameters : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 texCoord;

layout (std140, binding = 0) uniform FRAME_DATA
{
	mat4 view_matrix;
	mat4 proj_matrix;
};

layout (std430, binding = 0) readonly buffer OBJECT_TRANSFORMS
{
	mat4 model_matrix[];
};

out VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
	flat int material_id;
} vs_out;

uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

void main(void)
{
	mat4 mv_matrix = view_matrix * model_matrix[gl_DrawIDARB];
	vec4 P = mv_matrix * vec4(position, 1.0);

    vs_out.N = mat3(mv_matrix) * normal;

    vs_out.L = light_pos - P.xyz;

    vs_out.V = -P.xyz;

	vs_out.material_id = gl_BaseInstanceARB;
	gl_Position = proj_matrix * P;
}
